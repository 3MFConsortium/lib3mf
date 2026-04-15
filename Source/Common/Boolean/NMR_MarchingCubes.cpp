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

Marching-cubes iso-surface extraction for scalar fields.

--*/

#include "Common/Boolean/NMR_MarchingCubes.h"

#include "Common/NMR_Exception.h"

#include <vector>

namespace NMR::Boolean {

	namespace {
		std::array<double, 3> interpolateIso0(const std::array<double, 3> & p0, const std::array<double, 3> & p1, double v0, double v1)
		{
			const double t = (v0 == v1) ? 0.5 : (v0 / (v0 - v1));
			return {
				p0[0] + (p1[0] - p0[0]) * t,
				p0[1] + (p1[1] - p0[1]) * t,
				p0[2] + (p1[2] - p0[2]) * t
			};
		}

		static const int kMcEdgeTable[256] = {
			0x0, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
			0x190, 0x99, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
			0x230, 0x339, 0x33, 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
			0x3a0, 0x2a9, 0x1a3, 0xaa, 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
			0x460, 0x569, 0x663, 0x76a, 0x66, 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
			0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff, 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
			0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55, 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
			0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc, 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
			0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0xcc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
			0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x55, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
			0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0xff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
			0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x66, 0x76a, 0x663, 0x569, 0x460,
			0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa, 0x1a3, 0x2a9, 0x3a0,
			0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33, 0x339, 0x230,
			0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99, 0x190,
			0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
		};

#include "NMR_MarchingCubesTriTable.inc"
	}

	void extractIsoSurfaceMarchingCubes(
		_In_ CMesh * pResultMesh,
		_In_ const std::vector<double> & fieldValues,
		_In_ int resolution,
		_In_ const std::array<double, 3> & minCorner,
		_In_ const std::array<double, 3> & maxCorner)
	{
		if (!pResultMesh || resolution < 2)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const int nx = resolution - 1;
		const int ny = resolution - 1;
		const int nz = resolution - 1;
		const int vnx = resolution;
		const int vny = resolution;
		const int vnz = resolution;

		const std::array<double, 3> delta = {
			(maxCorner[0] - minCorner[0]) / static_cast<double>(resolution - 1),
			(maxCorner[1] - minCorner[1]) / static_cast<double>(resolution - 1),
			(maxCorner[2] - minCorner[2]) / static_cast<double>(resolution - 1)
		};

		auto valueIndex = [vnx, vny](int x, int y, int z) -> size_t {
			return static_cast<size_t>(x) +
				static_cast<size_t>(vnx) * (static_cast<size_t>(y) + static_cast<size_t>(vny) * static_cast<size_t>(z));
		};

		const int cornerOffset[8][3] = {
			{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
			{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}
		};
		const int edgeToCorners[12][2] = {
			{0, 1}, {1, 2}, {2, 3}, {3, 0},
			{4, 5}, {5, 6}, {6, 7}, {7, 4},
			{0, 4}, {1, 5}, {2, 6}, {3, 7}
		};

		const size_t xEdgeCount = static_cast<size_t>(nx) * static_cast<size_t>(vny) * static_cast<size_t>(vnz);
		const size_t yEdgeCount = static_cast<size_t>(vnx) * static_cast<size_t>(ny) * static_cast<size_t>(vnz);
		const size_t zEdgeCount = static_cast<size_t>(vnx) * static_cast<size_t>(vny) * static_cast<size_t>(nz);
		std::vector<int> xEdge(xEdgeCount, -1);
		std::vector<int> yEdge(yEdgeCount, -1);
		std::vector<int> zEdge(zEdgeCount, -1);
		std::vector<MESHNODE *> meshNodes;

		auto xEdgeIndex = [nx, vny](int ex, int ey, int ez) -> size_t {
			return static_cast<size_t>(ex) +
				static_cast<size_t>(nx) * (static_cast<size_t>(ey) + static_cast<size_t>(vny) * static_cast<size_t>(ez));
		};
		auto yEdgeIndex = [vnx, ny](int ex, int ey, int ez) -> size_t {
			return static_cast<size_t>(ex) +
				static_cast<size_t>(vnx) * (static_cast<size_t>(ey) + static_cast<size_t>(ny) * static_cast<size_t>(ez));
		};
		auto zEdgeIndex = [vnx, vny](int ex, int ey, int ez) -> size_t {
			return static_cast<size_t>(ex) +
				static_cast<size_t>(vnx) * (static_cast<size_t>(ey) + static_cast<size_t>(vny) * static_cast<size_t>(ez));
		};

		auto cornerValue = [&](int cx, int cy, int cz, int corner) -> double {
			const int gx = cx + cornerOffset[corner][0];
			const int gy = cy + cornerOffset[corner][1];
			const int gz = cz + cornerOffset[corner][2];
			return fieldValues[valueIndex(gx, gy, gz)];
		};

		auto cornerPosition = [&](int cx, int cy, int cz, int corner) -> std::array<double, 3> {
			const int gx = cx + cornerOffset[corner][0];
			const int gy = cy + cornerOffset[corner][1];
			const int gz = cz + cornerOffset[corner][2];
			return {
				minCorner[0] + static_cast<double>(gx) * delta[0],
				minCorner[1] + static_cast<double>(gy) * delta[1],
				minCorner[2] + static_cast<double>(gz) * delta[2]
			};
		};

		auto edgeVertex = [&](int cx, int cy, int cz, int edge) -> int {
			int * pSlot = nullptr;
			switch (edge) {
			case 0: pSlot = &xEdge[xEdgeIndex(cx, cy, cz)]; break;
			case 2: pSlot = &xEdge[xEdgeIndex(cx, cy + 1, cz)]; break;
			case 4: pSlot = &xEdge[xEdgeIndex(cx, cy, cz + 1)]; break;
			case 6: pSlot = &xEdge[xEdgeIndex(cx, cy + 1, cz + 1)]; break;
			case 3: pSlot = &yEdge[yEdgeIndex(cx, cy, cz)]; break;
			case 1: pSlot = &yEdge[yEdgeIndex(cx + 1, cy, cz)]; break;
			case 7: pSlot = &yEdge[yEdgeIndex(cx, cy, cz + 1)]; break;
			case 5: pSlot = &yEdge[yEdgeIndex(cx + 1, cy, cz + 1)]; break;
			case 8: pSlot = &zEdge[zEdgeIndex(cx, cy, cz)]; break;
			case 9: pSlot = &zEdge[zEdgeIndex(cx + 1, cy, cz)]; break;
			case 11: pSlot = &zEdge[zEdgeIndex(cx, cy + 1, cz)]; break;
			case 10: pSlot = &zEdge[zEdgeIndex(cx + 1, cy + 1, cz)]; break;
			default: break;
			}

			if (pSlot && *pSlot >= 0)
				return *pSlot;

			const int c0 = edgeToCorners[edge][0];
			const int c1 = edgeToCorners[edge][1];
			const auto p0 = cornerPosition(cx, cy, cz, c0);
			const auto p1 = cornerPosition(cx, cy, cz, c1);
			const double v0 = cornerValue(cx, cy, cz, c0);
			const double v1 = cornerValue(cx, cy, cz, c1);
			const auto p = interpolateIso0(p0, p1, v0, v1);

			auto * pNode = pResultMesh->addNode(static_cast<nfFloat>(p[0]), static_cast<nfFloat>(p[1]), static_cast<nfFloat>(p[2]));
			const int nNodeIdx = static_cast<int>(meshNodes.size());
			meshNodes.push_back(pNode);
			if (pSlot)
				*pSlot = nNodeIdx;
			return nNodeIdx;
		};

		for (int cz = 0; cz < nz; ++cz) {
			for (int cy = 0; cy < ny; ++cy) {
				for (int cx = 0; cx < nx; ++cx) {
					int cubeIndex = 0;
					for (int corner = 0; corner < 8; ++corner) {
						if (cornerValue(cx, cy, cz, corner) >= 0.0)
							cubeIndex |= (1 << corner);
					}

					const int edgeMask = kMcEdgeTable[cubeIndex];
					if (edgeMask == 0)
						continue;

					int edgeNodes[12] = { 0 };
					for (int edge = 0; edge < 12; ++edge) {
						if (edgeMask & (1 << edge))
							edgeNodes[edge] = edgeVertex(cx, cy, cz, edge);
					}

					const int * pTriangles = kMcTriTable[cubeIndex];
					for (int t = 0; pTriangles[t] != -1; t += 3) {
						const int ia = edgeNodes[pTriangles[t]];
						const int ib = edgeNodes[pTriangles[t + 1]];
						const int ic = edgeNodes[pTriangles[t + 2]];
						if (ia == ib || ib == ic || ic == ia)
							continue;
						pResultMesh->addFace(meshNodes[ia], meshNodes[ib], meshNodes[ic]);
					}
				}
			}
		}
	}

}
