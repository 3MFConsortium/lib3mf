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

NMR_MeshExporter_STL.cpp implements the Mesh Exporter Class.
This is a derived class for Exporting the binary STL and color STL Mesh Format.

--*/

#include "Common/MeshExport/NMR_MeshExporter_STL.h" 
#include "Common/MeshImport/NMR_MeshImporter_STL.h" 
#include "Common/Math/NMR_Matrix.h" 
#include "Common/Math/NMR_Vector.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>
#include <list>

namespace NMR {

	CMeshExporter_STL::CMeshExporter_STL() : CMeshExporter()
	{
		// empty on purpose
	}

	CMeshExporter_STL::CMeshExporter_STL(PExportStream pStream) : CMeshExporter(pStream)
	{
		// empty on purpose
	}

	void CMeshExporter_STL::exportMeshEx(_In_ CMesh * pMesh, _In_opt_ NMATRIX3 * pmMatrix, _In_opt_ CMeshExportEdgeMap * pExportEdgeMap)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CExportStream * pStream = getStream();
		if (!pStream)
			throw CNMRException(NMR_ERROR_NOEXPORTSTREAM);

		nfUint32 nIdx, j;
		nfUint32 nFaceCount = pMesh->getFaceCount();
		MESHFACE * face;
		MESHNODE * node;

		MESHFORMAT_STL_FACET facet;
		std::list<MESHFORMAT_STL_FACET> facetdata;

		for (nIdx = 0; nIdx < nFaceCount; nIdx++) {

			face = pMesh->getFace(nIdx);
			for (j = 0; j < 3; j++) {
				node = pMesh->getNode (face->m_nodeindices[j]);
				if (pmMatrix)
					facet.m_vertieces[j] = fnMATRIX3_apply(*pmMatrix, node->m_position);
				else
					facet.m_vertieces[j] = node->m_position;
			}	

			// Calculate Triangle Normals
			facet.m_normal = fnVEC3_calcTriangleNormal(facet.m_vertieces[0], facet.m_vertieces[1], facet.m_vertieces[2]);
			facet.m_attribute = 0;

			facetdata.push_back(facet);
		}

		nfByte stlheader[80];
		char HeaderMessage[34] = "STL Export by Lib3MF";
		nfUint32 nFacetCount = (nfUint32) facetdata.size();

		// Fill Header
		for (nIdx = 0; nIdx < 33; nIdx++)
			stlheader[nIdx] = (nfByte)HeaderMessage[nIdx];
		for (nIdx = 33; nIdx < 80; nIdx++)
			stlheader[nIdx] = 32;

		// Write Header
		pStream->writeBuffer(&stlheader[0], 80);
		pStream->writeBuffer(&nFacetCount, sizeof (nFacetCount));

		std::list<MESHFORMAT_STL_FACET>::iterator iter;
		for (iter = facetdata.begin(); iter != facetdata.end(); iter++) {
			pStream->writeBuffer(&(*iter), sizeof (MESHFORMAT_STL_FACET));
		}
	}

}
