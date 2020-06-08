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
#include "Common/Math/NMR_VectorTree.h" 
#include "Common/Math/NMR_Matrix.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>
#include <array>
#include <list>

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

		// TODO: handle colors
		//CMeshInformationHandler * pMeshInformationHandler = pMesh->createMeshInformationHandler();
		//CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType(0, emiProperties);
		//CMeshInformation_Properties * pProperties = nullptr;

		//if (!pProperties) {
		//	PMeshInformation_Properties pNewMeshInformation = std::make_shared<CMeshInformation_Properties>(pMesh->getFaceCount());
		//	pMeshInformationHandler->addInformation(pNewMeshInformation);

		//	pProperties = pNewMeshInformation.get();
		//}

		std::array<nfByte, 80> aSTLHeader;
		nfUint32 nFaceCount = 0;
		nfUint32 nGlobalColor = 0xffffffff;

		pStream->readBuffer(&aSTLHeader[0], 80, true);
		pStream->readBuffer((nfByte*)&nFaceCount, sizeof(nFaceCount), true);
		if (isBigEndian()) {
			nFaceCount = swapBytes(nFaceCount);
		}

		if (nFaceCount > NMR_MESH_MAXFACECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDFACECOUNT);

		std::string sHeaderString(std::begin(aSTLHeader), std::end(aSTLHeader));
		std::size_t nFound = sHeaderString.find("COLOR=");
		if (nFound != std::string::npos) {
			if (nFound <= 76) {
				nGlobalColor = ((nfUint32)aSTLHeader[nFound + 6]) + (((nfUint32)aSTLHeader[nFound + 7]) << 8) + (((nfUint32)aSTLHeader[nFound + 8]) << 16) +
					(((nfUint32)aSTLHeader[nFound + 9]) << 24);
			}
		}

		nfUint32 nNodeIdx;
		MESHNODE * pNodes[3];
		MESHFORMAT_STL_FACET Facet;
		CVectorTree VectorTree;
		nfBool bIsValid;

		VectorTree.setUnits(m_fUnits);

		for (nfUint32 nIdx = 0; nIdx < nFaceCount; nIdx++) {
			pStream->readBuffer((nfByte*)&Facet, sizeof(Facet), true);
			if (isBigEndian()) {
				Facet.swapByteOrder();
			}

			// Check, if Coordinates are in Valid Space
			bIsValid = true;
			for (nfUint32 j = 0; j < 3; j++)
				for (nfUint32 k = 0; k < 3; k++)
					bIsValid &= (fabs(Facet.m_vertices[j].m_fields[k]) < NMR_MESH_MAXCOORDINATE);

			// Identify Nodes via Tree
			if (bIsValid) {

				for (nfUint32 j = 0; j < 3; j++) {
					NVEC3 vPosition = Facet.m_vertices[j];
					if (pmMatrix)
						vPosition = fnMATRIX3_apply(*pmMatrix, vPosition);

					if (VectorTree.findVector3(vPosition, nNodeIdx)) {
						pNodes[j] = pMesh->getNode(nNodeIdx);
					}
					else {
						pNodes[j] = pMesh->addNode(vPosition);
						VectorTree.addVector3(pNodes[j]->m_position, (nfUint32)pNodes[j]->m_index);
					}
				}

				// check, if Nodes are separate
				bIsValid = (pNodes[0] != pNodes[1]) && (pNodes[0] != pNodes[2]) && (pNodes[1] != pNodes[2]);
			}

			// Throw "Invalid Exception"
			if ((!bIsValid) && !m_bIgnoreInvalidFaces)
				throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);

			if (bIsValid) {
				MESHFACE * pFace = pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);
				//if (pProperties) {
				//	nfUint32 nRed = (nfUint32) ((nfFloat) (Facet.m_attribute & 0x1f) / (255.0f / 31.0f));
				//	nfUint32 nGreen = (nfUint32)((nfFloat)((Facet.m_attribute >> 5) & 0x1f) / (255.0f / 31.0f));
				//	nfUint32 nBlue = (nfUint32)((nfFloat)((Facet.m_attribute >> 10) & 0x1f) / (255.0f / 31.0f));

				//	// MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(pFace->m_index);
				//}
			}
		}

	}

}