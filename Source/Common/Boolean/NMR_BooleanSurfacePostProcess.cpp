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

Mesh post-processing helpers for extracted boolean surfaces.

--*/

#include "Common/Boolean/NMR_BooleanSurfacePostProcess.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace NMR::Boolean {

	namespace {
		sBooleanVec3d operator+(const sBooleanVec3d & a, const sBooleanVec3d & b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
		sBooleanVec3d operator-(const sBooleanVec3d & a, const sBooleanVec3d & b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
		sBooleanVec3d operator*(const sBooleanVec3d & a, const double s) { return { a.x * s, a.y * s, a.z * s }; }

		double sampleFieldTrilinear(_In_ const sBooleanFieldData & fieldData, _In_ const sBooleanVec3d & p)
		{
			const int resolution = fieldData.resolution;
			const auto idx = [resolution](int x, int y, int z) -> size_t {
				return static_cast<size_t>(z) * resolution * resolution +
					static_cast<size_t>(y) * resolution +
					static_cast<size_t>(x);
			};

			const auto sampleCoord = [resolution](double t) -> std::pair<int, double> {
				const double scaled = std::clamp(t * static_cast<double>(resolution - 1), 0.0, static_cast<double>(resolution - 1));
				const int i0 = std::clamp(static_cast<int>(std::floor(scaled)), 0, resolution - 2);
				const double frac = scaled - static_cast<double>(i0);
				return { i0, frac };
			};

			const double dx = std::max(fieldData.maxCorner.x - fieldData.minCorner.x, 1e-9);
			const double dy = std::max(fieldData.maxCorner.y - fieldData.minCorner.y, 1e-9);
			const double dz = std::max(fieldData.maxCorner.z - fieldData.minCorner.z, 1e-9);
			const auto [ix, tx] = sampleCoord((p.x - fieldData.minCorner.x) / dx);
			const auto [iy, ty] = sampleCoord((p.y - fieldData.minCorner.y) / dy);
			const auto [iz, tz] = sampleCoord((p.z - fieldData.minCorner.z) / dz);

			const double c000 = fieldData.values[idx(ix, iy, iz)];
			const double c100 = fieldData.values[idx(ix + 1, iy, iz)];
			const double c010 = fieldData.values[idx(ix, iy + 1, iz)];
			const double c110 = fieldData.values[idx(ix + 1, iy + 1, iz)];
			const double c001 = fieldData.values[idx(ix, iy, iz + 1)];
			const double c101 = fieldData.values[idx(ix + 1, iy, iz + 1)];
			const double c011 = fieldData.values[idx(ix, iy + 1, iz + 1)];
			const double c111 = fieldData.values[idx(ix + 1, iy + 1, iz + 1)];

			const double c00 = c000 * (1.0 - tx) + c100 * tx;
			const double c10 = c010 * (1.0 - tx) + c110 * tx;
			const double c01 = c001 * (1.0 - tx) + c101 * tx;
			const double c11 = c011 * (1.0 - tx) + c111 * tx;
			const double c0 = c00 * (1.0 - ty) + c10 * ty;
			const double c1 = c01 * (1.0 - ty) + c11 * ty;
			return c0 * (1.0 - tz) + c1 * tz;
		}
	}

	void smoothAndProjectExtractedSurface(
		_In_ CMesh * pMesh,
		_In_ const sBooleanFieldData & fieldData)
	{
		if (!pMesh)
			return;

		const int resolution = fieldData.resolution;
		if (resolution < 3 || fieldData.values.empty())
			return;

		const nfUint32 nNodeCount = pMesh->getNodeCount();
		if (nNodeCount < 3)
			return;

		std::vector<std::vector<nfUint32>> neighbors(static_cast<size_t>(nNodeCount));
		const nfUint32 nFaceCount = pMesh->getFaceCount();
		for (nfUint32 f = 0; f < nFaceCount; ++f) {
			const auto * pFace = pMesh->getFace(f);
			const nfUint32 a = static_cast<nfUint32>(pFace->m_nodeindices[0]);
			const nfUint32 b = static_cast<nfUint32>(pFace->m_nodeindices[1]);
			const nfUint32 c = static_cast<nfUint32>(pFace->m_nodeindices[2]);
			if (a == b || b == c || c == a)
				continue;
			neighbors[a].push_back(b); neighbors[a].push_back(c);
			neighbors[b].push_back(a); neighbors[b].push_back(c);
			neighbors[c].push_back(a); neighbors[c].push_back(b);
		}

		for (auto & nn : neighbors) {
			std::sort(nn.begin(), nn.end());
			nn.erase(std::unique(nn.begin(), nn.end()), nn.end());
		}

		std::vector<sBooleanVec3d> positions(static_cast<size_t>(nNodeCount));
		for (nfUint32 i = 0; i < nNodeCount; ++i) {
			const auto * pNode = pMesh->getNode(i);
			positions[i] = {
				static_cast<double>(pNode->m_position.m_fields[0]),
				static_cast<double>(pNode->m_position.m_fields[1]),
				static_cast<double>(pNode->m_position.m_fields[2])
			};
		}

		auto laplacianStep = [&](double weight) {
			std::vector<sBooleanVec3d> updated = positions;
			for (nfUint32 i = 0; i < nNodeCount; ++i) {
				const auto & nn = neighbors[i];
				if (nn.empty())
					continue;
				sBooleanVec3d avg{ 0.0, 0.0, 0.0 };
				for (const auto n : nn)
					avg = avg + positions[n];
				const double invCount = 1.0 / static_cast<double>(nn.size());
				avg = avg * invCount;
				const sBooleanVec3d lap = avg - positions[i];
				updated[i] = positions[i] + lap * weight;
			}
			positions.swap(updated);
		};

		constexpr int smoothingIterations = 3;
		constexpr double lambda = 0.33;
		constexpr double mu = -0.34;
		for (int iter = 0; iter < smoothingIterations; ++iter) {
			laplacianStep(lambda);
			laplacianStep(mu);
		}

		const sBooleanVec3d cellStep = {
			(fieldData.maxCorner.x - fieldData.minCorner.x) / static_cast<double>(resolution - 1),
			(fieldData.maxCorner.y - fieldData.minCorner.y) / static_cast<double>(resolution - 1),
			(fieldData.maxCorner.z - fieldData.minCorner.z) / static_cast<double>(resolution - 1)
		};
		const sBooleanVec3d gradEps = {
			std::max(0.5 * cellStep.x, 1e-6),
			std::max(0.5 * cellStep.y, 1e-6),
			std::max(0.5 * cellStep.z, 1e-6)
		};

		constexpr int projectionIterations = 2;
		for (int iter = 0; iter < projectionIterations; ++iter) {
			for (nfUint32 i = 0; i < nNodeCount; ++i) {
				sBooleanVec3d p = positions[i];
				const double f = sampleFieldTrilinear(fieldData, p);
				if (std::abs(f) < 1e-5)
					continue;

				const sBooleanVec3d pxp{ p.x + gradEps.x, p.y, p.z };
				const sBooleanVec3d pxm{ p.x - gradEps.x, p.y, p.z };
				const sBooleanVec3d pyp{ p.x, p.y + gradEps.y, p.z };
				const sBooleanVec3d pym{ p.x, p.y - gradEps.y, p.z };
				const sBooleanVec3d pzp{ p.x, p.y, p.z + gradEps.z };
				const sBooleanVec3d pzm{ p.x, p.y, p.z - gradEps.z };

				const sBooleanVec3d grad{
					(sampleFieldTrilinear(fieldData, pxp) - sampleFieldTrilinear(fieldData, pxm)) / (2.0 * gradEps.x),
					(sampleFieldTrilinear(fieldData, pyp) - sampleFieldTrilinear(fieldData, pym)) / (2.0 * gradEps.y),
					(sampleFieldTrilinear(fieldData, pzp) - sampleFieldTrilinear(fieldData, pzm)) / (2.0 * gradEps.z)
				};

				const double gradNorm2 = grad.x * grad.x + grad.y * grad.y + grad.z * grad.z;
				if (gradNorm2 < 1e-12)
					continue;

				const double stepScale = std::clamp(f / gradNorm2, -2.0, 2.0);
				positions[i] = p - grad * stepScale;
			}
		}

		for (nfUint32 i = 0; i < nNodeCount; ++i) {
			auto * pNode = pMesh->getNode(i);
			pNode->m_position.m_fields[0] = static_cast<nfFloat>(positions[i].x);
			pNode->m_position.m_fields[1] = static_cast<nfFloat>(positions[i].y);
			pNode->m_position.m_fields[2] = static_cast<nfFloat>(positions[i].z);
		}
	}

}
