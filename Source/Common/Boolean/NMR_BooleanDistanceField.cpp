/*++

Copyright (C) 2026 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

Signed-distance field generation for winding-number based boolean CSG.

--*/

#include "Common/Boolean/NMR_BooleanDistanceField.h"

#include "Common/NMR_Exception.h"
#include "Common/Winding/NMR_WindingNumber.h"
#include "tiny_bvh.h"
#include "nanoflann.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

namespace NMR::Boolean {

	namespace {
		sBooleanVec3d operator+(const sBooleanVec3d & a, const sBooleanVec3d & b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
		sBooleanVec3d operator-(const sBooleanVec3d & a, const sBooleanVec3d & b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
		sBooleanVec3d operator*(const sBooleanVec3d & a, const double s) { return { a.x * s, a.y * s, a.z * s }; }
		double dot(const sBooleanVec3d & a, const sBooleanVec3d & b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

		double combineField(double a, double b, eModelBooleanOperation operation)
		{
			switch (operation) {
			case eModelBooleanOperation::Union:
				return std::min(a, b);
			case eModelBooleanOperation::Intersection:
				return std::max(a, b);
			case eModelBooleanOperation::Difference:
				return std::max(a, -b);
			default:
				return std::min(a, b);
			}
		}

		struct sDistanceMesh {
			struct sCentroidAdaptor {
				const std::vector<std::array<double, 3>> * pCentroids = nullptr;
				size_t kdtree_get_point_count() const { return pCentroids ? pCentroids->size() : 0; }
				double kdtree_get_pt(size_t idx, size_t dim) const { return (*pCentroids)[idx][dim]; }
				template<class BBOX> bool kdtree_get_bbox(BBOX &) const { return false; }
			};

			using CentroidKDTree = nanoflann::KDTreeSingleIndexAdaptor<
				nanoflann::L2_Simple_Adaptor<double, sCentroidAdaptor>,
				sCentroidAdaptor, 3, size_t>;

			std::vector<std::array<double, 3>> vertices;
			std::vector<std::array<uint32_t, 3>> triangles;
			std::vector<std::array<double, 3>> centroids;
			std::vector<tinybvh::bvhdbl3> triVertsPacked;
			sCentroidAdaptor centroidAdaptor;
			std::unique_ptr<CentroidKDTree> pCentroidKDTree;
			std::unique_ptr<tinybvh::BVH_Double> pBVH;

			sDistanceMesh() = default;
			sDistanceMesh(const sDistanceMesh &) = delete;
			sDistanceMesh & operator=(const sDistanceMesh &) = delete;

			sDistanceMesh(sDistanceMesh && other) noexcept
				: vertices(std::move(other.vertices)),
				triangles(std::move(other.triangles)),
				centroids(std::move(other.centroids)),
				triVertsPacked(std::move(other.triVertsPacked)),
				pBVH(std::move(other.pBVH))
			{
				rebuildCentroidKDTree();
			}

			sDistanceMesh & operator=(sDistanceMesh && other) noexcept
			{
				if (this != &other) {
					vertices = std::move(other.vertices);
					triangles = std::move(other.triangles);
					centroids = std::move(other.centroids);
					triVertsPacked = std::move(other.triVertsPacked);
					pBVH = std::move(other.pBVH);
					rebuildCentroidKDTree();
				}
				return *this;
			}

		private:
			void rebuildCentroidKDTree()
			{
				centroidAdaptor.pCentroids = nullptr;
				pCentroidKDTree.reset();
				if (!centroids.empty()) {
					centroidAdaptor.pCentroids = &centroids;
					pCentroidKDTree = std::make_unique<CentroidKDTree>(3, centroidAdaptor, nanoflann::KDTreeSingleIndexAdaptorParams(12));
					pCentroidKDTree->buildIndex();
				}
			}
		};

		struct sWindingMesh {
			std::vector<tg::windingNumber::Point> vertices;
			std::vector<tg::windingNumber::Triangle> triangles;
			std::unique_ptr<tg::windingNumber> winding;
		};

		sWindingMesh makeWindingMesh(_In_ CMesh * pMesh)
		{
			if (!pMesh)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pMesh->getNodeCount() < 3 || pMesh->getFaceCount() < 1)
				throw CNMRException(NMR_ERROR_INVALIDOBJECT);

			sWindingMesh result;
			result.vertices.resize(pMesh->getNodeCount());
			result.triangles.resize(pMesh->getFaceCount());

			for (nfUint32 nNodeIdx = 0; nNodeIdx < pMesh->getNodeCount(); ++nNodeIdx) {
				const auto * pNode = pMesh->getNode(nNodeIdx);
				result.vertices[nNodeIdx] = {
					static_cast<double>(pNode->m_position.m_fields[0]),
					static_cast<double>(pNode->m_position.m_fields[1]),
					static_cast<double>(pNode->m_position.m_fields[2])
				};
			}

			for (nfUint32 nFaceIdx = 0; nFaceIdx < pMesh->getFaceCount(); ++nFaceIdx) {
				const auto * pFace = pMesh->getFace(nFaceIdx);
				result.triangles[nFaceIdx] = {
					static_cast<unsigned int>(pFace->m_nodeindices[0]),
					static_cast<unsigned int>(pFace->m_nodeindices[1]),
					static_cast<unsigned int>(pFace->m_nodeindices[2])
				};
			}

			tg::windingNumber::Settings settings;
			settings.taylorOrder = 2;
			settings.accuracy = 2.0;
			result.winding = std::make_unique<tg::windingNumber>(result.vertices, result.triangles, settings);
			return result;
		}

		sDistanceMesh makeDistanceMesh(_In_ CMesh * pMesh)
		{
			if (!pMesh)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			sDistanceMesh result;
			result.vertices.resize(pMesh->getNodeCount());
			for (nfUint32 nNodeIdx = 0; nNodeIdx < pMesh->getNodeCount(); ++nNodeIdx) {
				const auto * pNode = pMesh->getNode(nNodeIdx);
				result.vertices[nNodeIdx] = {
					static_cast<double>(pNode->m_position.m_fields[0]),
					static_cast<double>(pNode->m_position.m_fields[1]),
					static_cast<double>(pNode->m_position.m_fields[2])
				};
			}

			result.triangles.resize(pMesh->getFaceCount());
			result.centroids.resize(pMesh->getFaceCount());
			result.triVertsPacked.reserve(static_cast<size_t>(pMesh->getFaceCount()) * 3ULL);
			for (nfUint32 nFaceIdx = 0; nFaceIdx < pMesh->getFaceCount(); ++nFaceIdx) {
				const auto * pFace = pMesh->getFace(nFaceIdx);
				const auto i0 = static_cast<uint32_t>(pFace->m_nodeindices[0]);
				const auto i1 = static_cast<uint32_t>(pFace->m_nodeindices[1]);
				const auto i2 = static_cast<uint32_t>(pFace->m_nodeindices[2]);
				result.triangles[nFaceIdx] = { i0, i1, i2 };

				const auto & v0 = result.vertices[i0];
				const auto & v1 = result.vertices[i1];
				const auto & v2 = result.vertices[i2];
				result.centroids[nFaceIdx] = {
					(v0[0] + v1[0] + v2[0]) / 3.0,
					(v0[1] + v1[1] + v2[1]) / 3.0,
					(v0[2] + v1[2] + v2[2]) / 3.0
				};
				result.triVertsPacked.emplace_back(v0[0], v0[1], v0[2]);
				result.triVertsPacked.emplace_back(v1[0], v1[1], v1[2]);
				result.triVertsPacked.emplace_back(v2[0], v2[1], v2[2]);
			}

			if (!result.centroids.empty()) {
				result.centroidAdaptor.pCentroids = &result.centroids;
				result.pCentroidKDTree = std::make_unique<sDistanceMesh::CentroidKDTree>(3, result.centroidAdaptor, nanoflann::KDTreeSingleIndexAdaptorParams(12));
				result.pCentroidKDTree->buildIndex();
			}

			if (!result.triangles.empty()) {
				result.pBVH = std::make_unique<tinybvh::BVH_Double>();
				result.pBVH->Build(result.triVertsPacked.data(), static_cast<uint64_t>(result.triangles.size()));
			}

			return result;
		}

		double pointTriangleDistanceSquared(const sBooleanVec3d & p, const sBooleanVec3d & a, const sBooleanVec3d & b, const sBooleanVec3d & c)
		{
			const sBooleanVec3d ab = b - a;
			const sBooleanVec3d ac = c - a;
			const sBooleanVec3d ap = p - a;

			const double d1 = dot(ab, ap);
			const double d2 = dot(ac, ap);
			if (d1 <= 0.0 && d2 <= 0.0)
				return dot(ap, ap);

			const sBooleanVec3d bp = p - b;
			const double d3 = dot(ab, bp);
			const double d4 = dot(ac, bp);
			if (d3 >= 0.0 && d4 <= d3)
				return dot(bp, bp);

			const double vc = d1 * d4 - d3 * d2;
			if (vc <= 0.0 && d1 >= 0.0 && d3 <= 0.0) {
				const double v = d1 / (d1 - d3);
				const sBooleanVec3d proj = a + ab * v;
				const sBooleanVec3d diff = p - proj;
				return dot(diff, diff);
			}

			const sBooleanVec3d cp = p - c;
			const double d5 = dot(ab, cp);
			const double d6 = dot(ac, cp);
			if (d6 >= 0.0 && d5 <= d6)
				return dot(cp, cp);

			const double vb = d5 * d2 - d1 * d6;
			if (vb <= 0.0 && d2 >= 0.0 && d6 <= 0.0) {
				const double w = d2 / (d2 - d6);
				const sBooleanVec3d proj = a + ac * w;
				const sBooleanVec3d diff = p - proj;
				return dot(diff, diff);
			}

			const double va = d3 * d6 - d5 * d4;
			if (va <= 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) >= 0.0) {
				const sBooleanVec3d bc = c - b;
				const double w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
				const sBooleanVec3d proj = b + bc * w;
				const sBooleanVec3d diff = p - proj;
				return dot(diff, diff);
			}

			const double denom = 1.0 / (va + vb + vc);
			const double v = vb * denom;
			const double w = vc * denom;
			const sBooleanVec3d proj = a + ab * v + ac * w;
			const sBooleanVec3d diff = p - proj;
			return dot(diff, diff);
		}

		double pointAABBDistanceSquared(const sBooleanVec3d & p, const tinybvh::bvhdbl3 & bmin, const tinybvh::bvhdbl3 & bmax)
		{
			const double dx = (p.x < bmin.x) ? (bmin.x - p.x) : ((p.x > bmax.x) ? (p.x - bmax.x) : 0.0);
			const double dy = (p.y < bmin.y) ? (bmin.y - p.y) : ((p.y > bmax.y) ? (p.y - bmax.y) : 0.0);
			const double dz = (p.z < bmin.z) ? (bmin.z - p.z) : ((p.z > bmax.z) ? (p.z - bmax.z) : 0.0);
			return dx * dx + dy * dy + dz * dz;
		}

		double unsignedDistanceToMesh(const sDistanceMesh & mesh, const sBooleanVec3d & p)
		{
			if (mesh.triangles.empty() || !mesh.pBVH || mesh.pBVH->usedNodes == 0 || mesh.pBVH->bvhNode == nullptr)
				return std::numeric_limits<double>::infinity();

			double minDist2 = std::numeric_limits<double>::infinity();

			if (mesh.pCentroidKDTree) {
				constexpr size_t kSeeds = 4;
				const size_t k = std::min(kSeeds, mesh.triangles.size());
				std::array<size_t, kSeeds> seedIndices{};
				std::array<double, kSeeds> seedDists{};
				nanoflann::KNNResultSet<double, size_t> resultSet(k);
				resultSet.init(seedIndices.data(), seedDists.data());
				const double query[3] = { p.x, p.y, p.z };
				mesh.pCentroidKDTree->findNeighbors(resultSet, query, nanoflann::SearchParameters(16));

				for (size_t i = 0; i < k; ++i) {
					const auto triId = seedIndices[i];
					const auto & tri = mesh.triangles[triId];
					const auto & av = mesh.vertices[tri[0]];
					const auto & bv = mesh.vertices[tri[1]];
					const auto & cv = mesh.vertices[tri[2]];
					const sBooleanVec3d a{ av[0], av[1], av[2] };
					const sBooleanVec3d b{ bv[0], bv[1], bv[2] };
					const sBooleanVec3d c{ cv[0], cv[1], cv[2] };
					minDist2 = std::min(minDist2, pointTriangleDistanceSquared(p, a, b, c));
				}
			}

			std::vector<std::pair<uint64_t, double>> stack;
			stack.reserve(256);

			uint64_t nodeIdx = 0;
			double nodeDist2 = pointAABBDistanceSquared(p, mesh.pBVH->bvhNode[0].aabbMin, mesh.pBVH->bvhNode[0].aabbMax);

			while (true) {
				if (nodeDist2 < minDist2) {
					const auto & node = mesh.pBVH->bvhNode[nodeIdx];
					if (node.isLeaf()) {
						for (uint64_t triOffset = 0; triOffset < node.triCount; ++triOffset) {
							const auto triId = mesh.pBVH->primIdx[node.leftFirst + triOffset];
							const auto & tri = mesh.triangles[triId];
							const auto & av = mesh.vertices[tri[0]];
							const auto & bv = mesh.vertices[tri[1]];
							const auto & cv = mesh.vertices[tri[2]];
							const sBooleanVec3d a{ av[0], av[1], av[2] };
							const sBooleanVec3d b{ bv[0], bv[1], bv[2] };
							const sBooleanVec3d c{ cv[0], cv[1], cv[2] };
							minDist2 = std::min(minDist2, pointTriangleDistanceSquared(p, a, b, c));
						}
					}
					else {
						const uint64_t leftIdx = node.leftFirst;
						const uint64_t rightIdx = node.leftFirst + 1;

						const auto & leftNode = mesh.pBVH->bvhNode[leftIdx];
						const auto & rightNode = mesh.pBVH->bvhNode[rightIdx];

						const double leftDist2 = pointAABBDistanceSquared(p, leftNode.aabbMin, leftNode.aabbMax);
						const double rightDist2 = pointAABBDistanceSquared(p, rightNode.aabbMin, rightNode.aabbMax);
						const uint64_t nearIdx = (leftDist2 <= rightDist2) ? leftIdx : rightIdx;
						const uint64_t farIdx = (leftDist2 <= rightDist2) ? rightIdx : leftIdx;
						const double nearDist2 = (leftDist2 <= rightDist2) ? leftDist2 : rightDist2;
						const double farDist2 = (leftDist2 <= rightDist2) ? rightDist2 : leftDist2;

						if (farDist2 < minDist2)
							stack.emplace_back(farIdx, farDist2);

						if (nearDist2 < minDist2) {
							nodeIdx = nearIdx;
							nodeDist2 = nearDist2;
							continue;
						}
					}
				}

				if (stack.empty())
					break;
				nodeIdx = stack.back().first;
				nodeDist2 = stack.back().second;
				stack.pop_back();
			}

			return std::sqrt(std::max(minDist2, 0.0));
		}

		double signedDistanceField(
			_In_ const tg::windingNumber & winding,
			_In_ const sDistanceMesh & distanceMesh,
			_In_ const sBooleanVec3d & point)
		{
			const auto w = winding.query({ point.x, point.y, point.z });
			const double sign = (std::abs(w) >= 0.5) ? -1.0 : 1.0;
			return sign * unsignedDistanceToMesh(distanceMesh, point);
		}

		void computeBounds(_In_ const std::vector<CMesh *> & meshes, _Out_ sBooleanVec3d & minCorner, _Out_ sBooleanVec3d & maxCorner)
		{
			minCorner = { 1e300, 1e300, 1e300 };
			maxCorner = { -1e300, -1e300, -1e300 };
			bool hasAnyNode = false;

			for (auto * pMesh : meshes) {
				if (!pMesh)
					continue;

				for (nfUint32 nNodeIdx = 0; nNodeIdx < pMesh->getNodeCount(); ++nNodeIdx) {
					const auto * pNode = pMesh->getNode(nNodeIdx);
					hasAnyNode = true;
					minCorner.x = std::min(minCorner.x, static_cast<double>(pNode->m_position.m_fields[0]));
					minCorner.y = std::min(minCorner.y, static_cast<double>(pNode->m_position.m_fields[1]));
					minCorner.z = std::min(minCorner.z, static_cast<double>(pNode->m_position.m_fields[2]));
					maxCorner.x = std::max(maxCorner.x, static_cast<double>(pNode->m_position.m_fields[0]));
					maxCorner.y = std::max(maxCorner.y, static_cast<double>(pNode->m_position.m_fields[1]));
					maxCorner.z = std::max(maxCorner.z, static_cast<double>(pNode->m_position.m_fields[2]));
				}
			}

			if (!hasAnyNode)
				throw CNMRException(NMR_ERROR_INVALIDOBJECT);

			const sBooleanVec3d size = maxCorner - minCorner;
			const double padding = std::max({ size.x, size.y, size.z, 1e-3 }) * 0.05;
			minCorner.x -= padding; minCorner.y -= padding; minCorner.z -= padding;
			maxCorner.x += padding; maxCorner.y += padding; maxCorner.z += padding;
		}
	}

	sBooleanFieldData buildCSGField(
		_In_ CMesh * pBaseMesh,
		_In_ const std::vector<PMesh> & operandMeshes,
		_In_ eModelBooleanOperation operation,
		_In_ nfUint32 nGridResolution)
	{
		if (!pBaseMesh || operandMeshes.empty())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto baseWinding = makeWindingMesh(pBaseMesh);
		auto baseDistance = makeDistanceMesh(pBaseMesh);
		std::vector<sWindingMesh> operandWindings;
		std::vector<sDistanceMesh> operandDistances;
		operandWindings.reserve(operandMeshes.size());
		operandDistances.reserve(operandMeshes.size());
		for (const auto & operandMesh : operandMeshes) {
			operandWindings.push_back(makeWindingMesh(operandMesh.get()));
			operandDistances.push_back(makeDistanceMesh(operandMesh.get()));
		}

		std::vector<CMesh *> allMeshes;
		allMeshes.reserve(operandMeshes.size() + 1);
		allMeshes.push_back(pBaseMesh);
		for (const auto & operandMesh : operandMeshes)
			allMeshes.push_back(operandMesh.get());

		sBooleanFieldData fieldData;
		computeBounds(allMeshes, fieldData.minCorner, fieldData.maxCorner);
		if (nGridResolution < 2 || nGridResolution > static_cast<nfUint32>(std::numeric_limits<int>::max()))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		fieldData.resolution = static_cast<int>(nGridResolution);
		fieldData.values.resize(static_cast<size_t>(fieldData.resolution) * fieldData.resolution * fieldData.resolution);

		const auto idx = [resolution = fieldData.resolution](int x, int y, int z) -> size_t {
			return static_cast<size_t>(z) * resolution * resolution +
				static_cast<size_t>(y) * resolution +
				static_cast<size_t>(x);
		};

		const sBooleanVec3d step = {
			(fieldData.maxCorner.x - fieldData.minCorner.x) / static_cast<double>(fieldData.resolution - 1),
			(fieldData.maxCorner.y - fieldData.minCorner.y) / static_cast<double>(fieldData.resolution - 1),
			(fieldData.maxCorner.z - fieldData.minCorner.z) / static_cast<double>(fieldData.resolution - 1)
		};

		for (int z = 0; z < fieldData.resolution; ++z) {
			for (int y = 0; y < fieldData.resolution; ++y) {
				for (int x = 0; x < fieldData.resolution; ++x) {
					const sBooleanVec3d p = {
						fieldData.minCorner.x + x * step.x,
						fieldData.minCorner.y + y * step.y,
						fieldData.minCorner.z + z * step.z
					};

					double csgField = signedDistanceField(*baseWinding.winding, baseDistance, p);
					for (size_t opIdx = 0; opIdx < operandWindings.size(); ++opIdx) {
						const double operandField = signedDistanceField(*operandWindings[opIdx].winding, operandDistances[opIdx], p);
						csgField = combineField(csgField, operandField, operation);
					}
					fieldData.values[idx(x, y, z)] = csgField;
				}
			}
		}

		return fieldData;
	}

}
