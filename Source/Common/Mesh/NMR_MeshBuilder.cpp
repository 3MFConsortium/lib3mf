/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_MeshBuilder.cpp implements the class CMeshBuilder.

The class CMeshBuilder is auxiliary class, which construct a valid mesh by adding single faces and nodes. 
In addition already existing meshes can be added.

--*/

#include "Common/Mesh/NMR_MeshBuilder.h" 
#include "Common/NMR_Exception.h" 
#include <math.h>

namespace NMR {

	CMeshBuilder::CMeshBuilder()
	{
		// empty on purpose
	}

	CMeshBuilder::CMeshBuilder(_In_ nfFloat fUnits)
	{
		m_VectorTree.setUnits(fUnits);
	}

	nfUint32 CMeshBuilder::addNode(_In_ const NVEC3 vPoint)
	{
		nfUint32 nIdx;
		
		if (m_VectorTree.findVector3(vPoint, nIdx))
			return nIdx;

		NVEC3 * pPoint = m_Nodes.allocData(nIdx);
		pPoint->m_values = vPoint.m_values;

		m_VectorTree.addVector3(vPoint, nIdx);

		return nIdx;
	}

	nfUint32 CMeshBuilder::addFace(_In_ const NVEC3 vPoint1, _In_ const NVEC3 vPoint2, _In_ const NVEC3 vPoint3)
	{
		NVEC3I vFaceVec;
		vFaceVec.m_fields[0] = addNode(vPoint1);
		vFaceVec.m_fields[1] = addNode(vPoint2);
		vFaceVec.m_fields[2] = addNode(vPoint3);

		nfUint32 nIdx;
		NVEC3I * pFace = m_Faces.allocData (nIdx);
		*pFace = vFaceVec;

		return nIdx;
	}

	PMesh CMeshBuilder::createMesh(_In_ nfBool bIgnoreInvalidFaces)
	{
		PMesh pResult (new CMesh);
		addToMesh(pResult.get(), bIgnoreInvalidFaces);

		return pResult;
	}

	void CMeshBuilder::addToMesh(_In_ CMesh * pMesh, _In_ nfBool bIgnoreInvalidFaces)
	{
		nfUint32 nIdx, j;
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::vector<MESHNODE *> pNodes;

		nfUint32 nNodeCount = m_Nodes.getCount();
		nfUint32 nFaceCount = m_Faces.getCount();
		pNodes.resize(nNodeCount);

		for (nIdx = 0; nIdx < nNodeCount; nIdx++) {
			NVEC3 * pPosition = m_Nodes.getData(nIdx);
			pNodes[nIdx] = pMesh->addNode(*pPosition);
		}

		for (nIdx = 0; nIdx < nFaceCount; nIdx++) {
			NVEC3I * pFaceVec = m_Faces.getData(nIdx);		
			MESHNODE * pNewNodes[3];

			for (j = 0; j < 3; j++) {
				pNewNodes[j] = pNodes[pFaceVec->m_fields[j]];
			}

			if ((pNewNodes[0] == pNewNodes[1]) || (pNewNodes[0] == pNewNodes[2]) || (pNewNodes[1] == pNewNodes[2])) {
				if (!bIgnoreInvalidFaces)
					throw CNMRException(NMR_ERROR_DUPLICATENODE);

			}
			else {
				pMesh->addFace(pNewNodes[0], pNewNodes[1], pNewNodes[2]);
			}
		}
	}

}
