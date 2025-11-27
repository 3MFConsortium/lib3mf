/*++

Copyright (C) 2019 3MF Consortium

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

NMR_MeshImporter_STL.cpp implements the Mesh Importer Class.
This is a derived class for Importing the binary STL and color STL Mesh Format.

--*/

#include "Common/MeshImport/NMR_MeshImporter_STL.h"
#include "Common/MeshInformation/NMR_MeshInformation.h"
#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Common/Math/NMR_VectorTree.h"
#include "Common/Math/NMR_Matrix.h"
#include "Common/Math/NMR_Vector.h"
#include "Common/NMR_Exception.h"
#include <cmath>
#include <array>
#include <list>
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <limits>

namespace {

	std::string toLowerCopy(const std::string & text)
	{
		std::string lowered = text;
		std::transform(lowered.begin(), lowered.end(), lowered.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return lowered;
	}

	bool headerStartsWithSolid(const std::string & header)
	{
		auto firstNonWhitespace = header.find_first_not_of(" \t\r\n");
		if (firstNonWhitespace == std::string::npos)
			return false;

		const char keyword[] = "solid";
		for (size_t i = 0; i < 5; ++i) {
			if (firstNonWhitespace + i >= header.size())
				return false;
			if (std::tolower(static_cast<unsigned char>(header[firstNonWhitespace + i])) != keyword[i])
				return false;
		}
		return true;
	}

}

namespace NMR {

	CMeshImporter_STL::CMeshImporter_STL() : CMeshImporter()
	{
		setUnits(NMR_VECTOR_DEFAULTUNITS);
		setIgnoreInvalidFaces(true);
		setImportColors(false);
	}

	CMeshImporter_STL::CMeshImporter_STL(_In_ PImportStream pStream) : CMeshImporter(pStream)
	{
		setUnits(NMR_VECTOR_DEFAULTUNITS);
		setIgnoreInvalidFaces(true);
		setImportColors(false);
	}

	CMeshImporter_STL::CMeshImporter_STL(_In_ PImportStream pStream, _In_ nfFloat fUnits) : CMeshImporter(pStream)
	{
		setUnits(fUnits);
		setIgnoreInvalidFaces(true);
		setImportColors(false);
	}

	CMeshImporter_STL::CMeshImporter_STL(_In_ PImportStream pStream, _In_ nfFloat fUnits, _In_ nfBool bImportColors) : CMeshImporter(pStream)
	{
		setUnits(fUnits);
		setIgnoreInvalidFaces(true);
		setImportColors(bImportColors);
	}

	void CMeshImporter_STL::setUnits(_In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		m_fUnits = fUnits;
	}

	nfFloat CMeshImporter_STL::getUnits()
	{
		return m_fUnits;
	}

	void CMeshImporter_STL::setIgnoreInvalidFaces(_In_ nfBool bIgnoreInvalidFaces)
	{
		m_bIgnoreInvalidFaces = bIgnoreInvalidFaces;
	}

	nfBool CMeshImporter_STL::getIgnoreInvalidFaces()
	{
		return m_bIgnoreInvalidFaces;
	}

	void CMeshImporter_STL::setImportColors(_In_ nfBool bImportColors)
	{
		m_bImportColors = bImportColors;
	}

	nfBool CMeshImporter_STL::getImportColors()
	{
		return m_bImportColors;
	}

	void CMeshImporter_STL::loadMesh(_In_ CMesh * pMesh, _In_opt_ NMATRIX3 * pmMatrix)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CImportStream * pStream = getStream();
		if (!pStream)
			throw CNMRException(NMR_ERROR_NOIMPORTSTREAM);

		std::array<nfByte, 80> aSTLHeader;
		nfUint32 nFaceCount = 0;

		pStream->readIntoBuffer(aSTLHeader.data(), aSTLHeader.size(), true);
		pStream->readIntoBuffer(reinterpret_cast<nfByte*>(&nFaceCount), sizeof(nFaceCount), true);
		if (isBigEndian())
			nFaceCount = swapBytes(nFaceCount);

		const bool bCountValid = (nFaceCount <= NMR_MESH_MAXFACECOUNT);
		nfUint64 nStreamSize = pStream->retrieveSize();
		nfUint64 nExpectedBinarySize = 0;
		if (bCountValid)
			nExpectedBinarySize = 84 + (nfUint64)nFaceCount * 50;
		std::string headerString(reinterpret_cast<const char*>(aSTLHeader.data()), aSTLHeader.size());

		bool bTreatAsASCII = false;
		if ((nStreamSize > 0) && bCountValid && (nExpectedBinarySize != nStreamSize))
			bTreatAsASCII = true;
		else if (headerStartsWithSolid(headerString))
			bTreatAsASCII = true;

		if ((nStreamSize > 0) && bCountValid && (nExpectedBinarySize == nStreamSize))
			bTreatAsASCII = false;

		if (!bTreatAsASCII && !bCountValid)
			throw CNMRException(NMR_ERROR_INVALIDFACECOUNT);

		CVectorTree VectorTree;
		VectorTree.setUnits(m_fUnits);
		const nfFloat fDegenerateEpsilon = NMR_DEGENERATE_TRIANGLE_EPSILON;

		auto processTriangle = [&](const std::array<NVEC3, 3> & vertices, nfUint32 nElementIndex)
		{
			MESHNODE* pNodes[3];
			nfUint32 nNodeIdx = 0;
			bool bIsValid = true;

			for (nfUint32 j = 0; j < 3 && bIsValid; ++j) {
				NVEC3 vPosition = vertices[j];
				if (pmMatrix)
					vPosition = fnMATRIX3_apply(*pmMatrix, vPosition);

				for (nfUint32 k = 0; k < 3; k++) {
					if (fabs(vPosition.m_fields[k]) > NMR_MESH_MAXCOORDINATE) {
						bIsValid = false;
						break;
					}
				}
				if (!bIsValid)
					break;

				if (VectorTree.findVector3(vPosition, nNodeIdx)) {
					pNodes[j] = pMesh->getNode(nNodeIdx);
				}
				else {
					pNodes[j] = pMesh->addNode(vPosition);
					VectorTree.addVector3(pNodes[j]->m_position, (nfUint32)pNodes[j]->m_index);
				}
			}

			if (!bIsValid) {
				if (!m_bIgnoreInvalidFaces)
					throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);
				return;
			}

			bool bHasDuplicateNodes = (pNodes[0] == pNodes[1]) || (pNodes[0] == pNodes[2]) || (pNodes[1] == pNodes[2]);
			if (bHasDuplicateNodes) {
				if (!m_bIgnoreInvalidFaces)
					throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);

				pMesh->addDegenerateTriangle(nElementIndex, pNodes[0]->m_index, pNodes[1]->m_index, pNodes[2]->m_index);
				return;
			}

			NVEC3 vEdge1 = fnVEC3_sub(pNodes[1]->m_position, pNodes[0]->m_position);
			NVEC3 vEdge2 = fnVEC3_sub(pNodes[2]->m_position, pNodes[0]->m_position);
			NVEC3 vNormal = fnVEC3_crossproduct(vEdge1, vEdge2);

			nfFloat fNormalSquared =
				(vNormal.m_fields[0] * vNormal.m_fields[0]) +
				(vNormal.m_fields[1] * vNormal.m_fields[1]) +
				(vNormal.m_fields[2] * vNormal.m_fields[2]);

			if (fNormalSquared <= fDegenerateEpsilon) {
				if (!m_bIgnoreInvalidFaces)
					throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);

				pMesh->addDegenerateTriangle(nElementIndex, pNodes[0]->m_index, pNodes[1]->m_index, pNodes[2]->m_index);
				return;
			}

			pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);
		};

		if (bTreatAsASCII) {
			pStream->seekPosition(0, true);
			nfUint64 nAsciiSize = pStream->retrieveSize();
			if (nAsciiSize == 0)
				throw CNMRException(NMR_ERROR_INVALIDMESH);

			if (nAsciiSize > static_cast<nfUint64>(std::numeric_limits<size_t>::max()))
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			std::string asciiData;
			asciiData.resize(static_cast<size_t>(nAsciiSize));
			pStream->readIntoBuffer(reinterpret_cast<nfByte*>(&asciiData[0]), nAsciiSize, true);

			std::istringstream asciiStream(asciiData);
			std::string token;
			if (!(asciiStream >> token))
				throw CNMRException(NMR_ERROR_INVALIDMESH);

			if (toLowerCopy(token) != "solid")
				throw CNMRException(NMR_ERROR_INVALIDMESH);
			std::string restOfLine;
			std::getline(asciiStream, restOfLine);

			nfUint32 nElementIndex = 0;
			while (asciiStream >> token) {
				std::string tokenLower = toLowerCopy(token);
				if (tokenLower == "facet") {
					std::string normalToken;
					if (!(asciiStream >> normalToken) || toLowerCopy(normalToken) != "normal")
						throw CNMRException(NMR_ERROR_INVALIDMESH);

					nfDouble nx, ny, nz;
					if (!(asciiStream >> nx >> ny >> nz))
						throw CNMRException(NMR_ERROR_INVALIDMESH);

					std::string outerToken, loopToken;
					if (!(asciiStream >> outerToken >> loopToken) ||
						toLowerCopy(outerToken) != "outer" || toLowerCopy(loopToken) != "loop")
						throw CNMRException(NMR_ERROR_INVALIDMESH);

					std::array<NVEC3, 3> vertices;
					for (nfUint32 j = 0; j < 3; ++j) {
						std::string vertexToken;
						if (!(asciiStream >> vertexToken) || toLowerCopy(vertexToken) != "vertex")
							throw CNMRException(NMR_ERROR_INVALIDMESH);

						nfDouble x, y, z;
						if (!(asciiStream >> x >> y >> z))
							throw CNMRException(NMR_ERROR_INVALIDMESH);

						vertices[j] = fnVEC3_make(static_cast<nfFloat>(x), static_cast<nfFloat>(y), static_cast<nfFloat>(z));
					}

					std::string endLoopToken;
					if (!(asciiStream >> endLoopToken) || toLowerCopy(endLoopToken) != "endloop")
						throw CNMRException(NMR_ERROR_INVALIDMESH);

					std::string endFacetToken;
					if (!(asciiStream >> endFacetToken) || toLowerCopy(endFacetToken) != "endfacet")
						throw CNMRException(NMR_ERROR_INVALIDMESH);

					processTriangle(vertices, nElementIndex);
					++nElementIndex;
				}
				else if (tokenLower == "endsolid") {
					break;
				}
				else if (tokenLower == "solid") {
					std::getline(asciiStream, restOfLine);
				}
				else {
					std::getline(asciiStream, restOfLine);
				}
			}

			return;
		}

		MESHFORMAT_STL_FACET Facet;
		for (nfUint32 nIdx = 0; nIdx < nFaceCount; nIdx++) {
			pStream->readIntoBuffer((nfByte*)&Facet, sizeof(Facet), true);
			if (isBigEndian())
				Facet.swapByteOrder();

			std::array<NVEC3, 3> vertices = { Facet.m_vertices[0], Facet.m_vertices[1], Facet.m_vertices[2] };
			processTriangle(vertices, nIdx);
		}
	}

}
