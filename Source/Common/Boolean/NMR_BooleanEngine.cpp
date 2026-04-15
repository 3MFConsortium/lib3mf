/*++

Copyright (C) 2026 tensorGrad Consulting LLP

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

Winding-number based boolean field construction and CSG composition.

--*/

#include "Common/Boolean/NMR_BooleanEngine.h"

#include "Common/Boolean/NMR_MarchingCubes.h"
#include "Common/Winding/NMR_WindingNumber.h"
#include "Common/NMR_Exception.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>

namespace NMR {

	namespace {
		struct sVec3d {
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;
		};

		sVec3d operator-(const sVec3d & a, const sVec3d & b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

		double combineField(double a, double b, eModelBooleanOperation operation)
		{
			switch (operation) {
			case eModelBooleanOperation::Union:
				return std::max(a, b);
			case eModelBooleanOperation::Intersection:
				return std::min(a, b);
			case eModelBooleanOperation::Difference:
				return std::min(a, -b);
			default:
				return std::max(a, b);
			}
		}

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

		void computeBounds(_In_ const std::vector<CMesh *> & meshes, _Out_ sVec3d & minCorner, _Out_ sVec3d & maxCorner)
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

			const sVec3d size = maxCorner - minCorner;
			const double padding = std::max({ size.x, size.y, size.z, 1e-3 }) * 0.05;
			minCorner.x -= padding; minCorner.y -= padding; minCorner.z -= padding;
			maxCorner.x += padding; maxCorner.y += padding; maxCorner.z += padding;
		}

		double signedWindingField(_In_ const tg::windingNumber & winding, _In_ const sVec3d & point)
		{
			const auto value = winding.query({ point.x, point.y, point.z });
			return std::abs(value) - 0.5;
		}
	}

	void CBooleanEngine::evaluate(
		_In_ CMesh * pBaseMesh,
		_In_ const std::vector<PMesh> & operandMeshes,
		_In_ eModelBooleanOperation operation,
		_In_ CMesh * pResultMesh,
		_In_ nfUint32 nGridResolution)
	{
		if (!pBaseMesh || !pResultMesh || operandMeshes.empty())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto baseWinding = makeWindingMesh(pBaseMesh);
		std::vector<sWindingMesh> operandWindings;
		operandWindings.reserve(operandMeshes.size());
		for (const auto & operandMesh : operandMeshes)
			operandWindings.push_back(makeWindingMesh(operandMesh.get()));

		std::vector<CMesh *> allMeshes;
		allMeshes.reserve(operandMeshes.size() + 1);
		allMeshes.push_back(pBaseMesh);
		for (const auto & operandMesh : operandMeshes)
			allMeshes.push_back(operandMesh.get());

		sVec3d minCorner, maxCorner;
		computeBounds(allMeshes, minCorner, maxCorner);

		const int resolution = std::clamp(static_cast<int>(nGridResolution), 24, 256);
		std::vector<double> fieldValues(static_cast<size_t>(resolution) * resolution * resolution);

		auto idx = [resolution](int x, int y, int z) -> size_t {
			return static_cast<size_t>(z) * resolution * resolution +
				static_cast<size_t>(y) * resolution +
				static_cast<size_t>(x);
		};

		const sVec3d step = {
			(maxCorner.x - minCorner.x) / static_cast<double>(resolution - 1),
			(maxCorner.y - minCorner.y) / static_cast<double>(resolution - 1),
			(maxCorner.z - minCorner.z) / static_cast<double>(resolution - 1)
		};

		for (int z = 0; z < resolution; ++z) {
			for (int y = 0; y < resolution; ++y) {
				for (int x = 0; x < resolution; ++x) {
					const sVec3d p = {
						minCorner.x + x * step.x,
						minCorner.y + y * step.y,
						minCorner.z + z * step.z
					};

					double csgField = signedWindingField(*baseWinding.winding, p);
					for (const auto & operand : operandWindings) {
						const double operandField = signedWindingField(*operand.winding, p);
						csgField = combineField(csgField, operandField, operation);
					}
					fieldValues[idx(x, y, z)] = csgField;
				}
			}
		}

		Boolean::extractIsoSurfaceMarchingCubes(
			pResultMesh,
			fieldValues,
			resolution,
			{ minCorner.x, minCorner.y, minCorner.z },
			{ maxCorner.x, maxCorner.y, maxCorner.z });
	}

}
