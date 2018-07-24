/*++

Copyright (C) 2018 3MF Consortium

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

NMR_Mesh.cpp implements the class CMesh.

The class CMesh is not really a mesh, since it lacks the component edges and the
topological information. It only holds the nodes and the faces (triangles).
Each node,  and face have a ID, which allows to identify them. Each face have an
orientation (i.e. the face can look up or look down) and have three nodes.
The orientation is defined by the order of its nodes.

You can only add nodes and faces to mesh. You cannot remove the existing
structure.

--*/

#include "Common/Mesh/NMR_Mesh.h"
#include "Common/Math/NMR_Matrix.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>

namespace NMR {

	CMesh::CMesh(): m_BeamLattice(this->m_Nodes)
	{
		// empty on purpose
	}

	CMesh::CMesh(_In_opt_ CMesh * pMesh) : m_BeamLattice(this->m_Nodes)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		mergeMesh(pMesh);
	}

	void CMesh::mergeMesh(_In_opt_ CMesh * pMesh)
	{
		mergeMesh(pMesh, fnMATRIX3_identity());
	}
	
	void CMesh::mergeMesh(_In_opt_ CMesh * pMesh, _In_ NMATRIX3 mMatrix)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfInt32 nIdx, nNodeCount, nFaceCount, nBeamCount, j;
		MESHNODE * pNode;
		MESHFACE * pFace;
		MESHBEAM * pBeam;
		MESHNODE * pFaceNodes[3];
		MESHNODE * pBeamNodes[2];

		// Copy Mesh Information
		CMeshInformationHandler * pOtherMeshInformationHandler = pMesh->getMeshInformationHandler();
		if (pOtherMeshInformationHandler) {
			createMeshInformationHandler();
			m_pMeshInformationHandler->addInfoTableFrom(pOtherMeshInformationHandler, getFaceCount ());
		}

		nNodeCount = pMesh->getNodeCount();
		nFaceCount = pMesh->getFaceCount();
		nBeamCount = pMesh->getBeamCount();

		if (nNodeCount > 0) {
			std::vector<MESHNODE *> aNewNodes;
			aNewNodes.resize(nNodeCount);

			for (nIdx = 0; nIdx < nNodeCount; nIdx++) {
				pNode = pMesh->getNode(nIdx);
				NVEC3 vPosition = fnMATRIX3_apply(mMatrix, pNode->m_position);
				aNewNodes[nIdx] = addNode(vPosition);
			}

			if (nFaceCount > 0) {
				for (nIdx = 0; nIdx < nFaceCount; nIdx++) {
					pFace = pMesh->getFace(nIdx);
					for (j = 0; j < 3; j++) {
						if ((pFace->m_nodeindices[j] < 0) || (pFace->m_nodeindices[j] >= nNodeCount))
							throw CNMRException(NMR_ERROR_INVALIDNODEINDEX);

						pFaceNodes[j] = aNewNodes[pFace->m_nodeindices[j]];
					}

					MESHFACE * pNewFace = addFace(pFaceNodes[0], pFaceNodes[1], pFaceNodes[2]);
					if (m_pMeshInformationHandler && pOtherMeshInformationHandler) {
						m_pMeshInformationHandler->cloneFaceInfosFrom(pNewFace->m_index, pOtherMeshInformationHandler, pFace->m_index);
					}
				}
			}
			if (nBeamCount > 0) {
				for (nIdx = 0; nIdx < nBeamCount; nIdx++) {
					pBeam = pMesh->getBeam(nIdx);
					for (j = 0; j < 2; j++) {
						if ((pBeam->m_nodeindices[j] < 0) || (pBeam->m_nodeindices[j] >= nNodeCount))
							throw CNMRException(NMR_ERROR_INVALIDNODEINDEX);

						pBeamNodes[j] = aNewNodes[pBeam->m_nodeindices[j]];
					}
					addBeam(pBeamNodes[0], pBeamNodes[1], &pBeam->m_radius[0], &pBeam->m_radius[1], &pBeam->m_capMode[0], &pBeam->m_capMode[1]);
				}
			}

		}
	}

	void CMesh::addToMesh(_In_opt_ CMesh * pMesh)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pMesh->mergeMesh(this, fnMATRIX3_identity());
	}

	void CMesh::addToMesh(_In_opt_ CMesh * pMesh, _In_ NMATRIX3 mMatrix)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pMesh->mergeMesh(this, mMatrix);
	}

	_Ret_notnull_ MESHNODE * CMesh::addNode(_In_ const NVEC3 vPosition)
	{
		MESHNODE * pNode;
		nfUint32 j;

		// Check Position Validity
		for (j = 0; j < 3; j++)
			if (fabs(vPosition.m_fields[j]) > NMR_MESH_MAXCOORDINATE)
				throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);

		// Check Node Quota
		nfUint32 nNodeCount = getNodeCount();
		if (nNodeCount > NMR_MESH_MAXNODECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYNODES);

		// Allocate Data
		nfUint32 nNewIndex;
		pNode = m_Nodes.allocData(nNewIndex);
		pNode->m_index = nNewIndex;
		pNode->m_position = vPosition;

		return pNode;
	}

	_Ret_notnull_ MESHFACE * CMesh::addFace(_In_ MESHNODE * pNode1, _In_ MESHNODE * pNode2, _In_ MESHNODE * pNode3)
	{
		if ((!pNode1) || (!pNode2) || (!pNode3))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if ((pNode1 == pNode2) || (pNode1 == pNode3) || (pNode2 == pNode3))
			throw CNMRException(NMR_ERROR_DUPLICATENODE);

		MESHFACE * pFace;
		nfUint32 nFaceCount = getFaceCount ();

		if (nFaceCount > NMR_MESH_MAXFACECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYFACES);

		nfUint32 nNewIndex;

		pFace = m_Faces.allocData (nNewIndex);
		pFace->m_nodeindices[0] = pNode1->m_index;
		pFace->m_nodeindices[1] = pNode2->m_index;
		pFace->m_nodeindices[2] = pNode3->m_index;
		pFace->m_index = nNewIndex;

		if (m_pMeshInformationHandler)
			m_pMeshInformationHandler->addFace(getFaceCount ());

		return pFace;
	}

	_Ret_notnull_ MESHBEAM * CMesh::addBeam(_In_ MESHNODE * pNode1, _In_ MESHNODE * pNode2,
		_In_ nfDouble * pRadius1, _In_ nfDouble * pRadius2,
		_In_ nfInt32 * peCapMode1, _In_ nfInt32 * peCapMode2)
	{
		if ((!pNode1) || (!pNode2) || (!pRadius1) || (!pRadius2) || (!peCapMode1) || (!peCapMode2))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (pNode1 == pNode2)
			throw CNMRException(NMR_ERROR_DUPLICATENODE);

		MESHBEAM * pBeam;
		nfUint32 nBeamCount = getBeamCount();

		if (nBeamCount > NMR_MESH_MAXBEAMCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYBEAMS);

		nfUint32 nNewIndex;

		pBeam = m_BeamLattice.m_Beams.allocData(nNewIndex);
		pBeam->m_nodeindices[0] = pNode1->m_index;
		pBeam->m_nodeindices[1] = pNode2->m_index;
		pBeam->m_index = nNewIndex;
		pBeam->m_radius[0] = *pRadius1;
		pBeam->m_radius[1] = *pRadius2;

		pBeam->m_capMode[0] = *peCapMode1;
		pBeam->m_capMode[1] = *peCapMode2;

		return pBeam;
	}

	PBEAMSET CMesh::addBeamSet()
	{
		m_BeamLattice.m_pBeamSets.push_back(std::make_shared<BEAMSET>());
		return m_BeamLattice.m_pBeamSets.back();
	}


	nfUint32 CMesh::getNodeCount()	{
		return m_Nodes.getCount ();
	}

	nfUint32 CMesh::getFaceCount()
	{
		return m_Faces.getCount ();
	}

	nfUint32 CMesh::getBeamCount()
	{
		return m_BeamLattice.m_Beams.getCount();
	}

	nfUint32 CMesh::getBeamSetCount()
	{
		return (nfUint32)(m_BeamLattice.m_pBeamSets.size());
	}

	_Ret_notnull_ MESHNODE * CMesh::getNode(_In_ nfUint32 nIdx)
	{
		return m_Nodes.getData(nIdx);
	}

	_Ret_notnull_ MESHFACE * CMesh::getFace(_In_ nfUint32 nIdx)
	{
		return m_Faces.getData(nIdx);
	}

	_Ret_notnull_ MESHBEAM * CMesh::getBeam(_In_ nfUint32 nIdx)
	{
		return m_BeamLattice.m_Beams.getData(nIdx);
	}

	_Ret_notnull_ PBEAMSET CMesh::getBeamSet(_In_ nfUint32 nIdx)
	{
		if (nIdx >= m_BeamLattice.m_pBeamSets.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_BeamLattice.m_pBeamSets[nIdx];
	}

	void CMesh::setBeamLatticeMinLength(nfDouble dMinLength)
	{
		m_BeamLattice.m_dMinLength = dMinLength;
	}

	nfDouble CMesh::getBeamLatticeMinLength()
	{
		return m_BeamLattice.m_dMinLength;
	}

	void CMesh::setBeamLatticeCapMode(eModelBeamLatticeCapMode eCapMode)
	{
		m_BeamLattice.m_CapMode = eCapMode;
	}

	eModelBeamLatticeCapMode CMesh::getBeamLatticeCapMode()
	{
		return m_BeamLattice.m_CapMode;
	}

	void CMesh::setDefaultBeamRadius(nfDouble dRadius)
	{
		m_BeamLattice.m_dRadius = dRadius;
	}

	nfDouble CMesh::getDefaultBeamRadius()
	{
		return m_BeamLattice.m_dRadius;
	}

	nfBool CMesh::checkSanity()
	{
		nfUint32 nIdx, j;

		nfUint32 nNodeCount = getNodeCount();
		nfUint32 nFaceCount = getFaceCount();
		nfUint32 nBeamCount = getBeamCount();

		// max 2 billion Nodes/Faces
		if (nNodeCount > NMR_MESH_MAXNODECOUNT)
			return false;
		if (nFaceCount > NMR_MESH_MAXFACECOUNT)
			return false;
		if (nBeamCount > NMR_MESH_MAXBEAMCOUNT)
			return false;

		for (nIdx = 0; nIdx < nNodeCount; nIdx++) {
			MESHNODE * node = getNode(nIdx);
			if (node->m_index != (nfInt32) nIdx)
				return false;
			for (j = 0; j < 3; j++)
				if (fabs(node->m_position.m_fields[j]) > NMR_MESH_MAXCOORDINATE)
					return false;
		}

		for (nIdx = 0; nIdx < nFaceCount; nIdx++) {
			MESHFACE * face = getFace(nIdx);
			for (j = 0; j < 3; j++)
				if ((face->m_nodeindices[j] < 0) || (((nfUint32)face->m_nodeindices[j]) >= nNodeCount))
					return false;

			if ((face->m_nodeindices[0] == face->m_nodeindices[1]) ||
				(face->m_nodeindices[0] == face->m_nodeindices[2]) ||
				(face->m_nodeindices[1] == face->m_nodeindices[2]))
				return false;
		}

		for (nIdx = 0; nIdx < nBeamCount; nIdx++) {
			MESHBEAM * beam = getBeam(nIdx);
			for (j = 0; j < 2; j++)
				if ((beam->m_nodeindices[j] < 0) || (((nfUint32)beam->m_nodeindices[j]) >= nNodeCount))
					return false;

			if (beam->m_nodeindices[0] == beam->m_nodeindices[1])
				return false;
		}
		return true;
	}

	void CMesh::clear()
	{
		m_pMeshInformationHandler = NULL;
		m_Faces.clearAllData();
		m_Nodes.clearAllData();
		clearBeamLattice();
	}
	
	void CMesh::clearBeamLattice() {
		m_BeamLattice.clear();
	}

	void CMesh::clearMeshInformationHandler()
	{
		m_pMeshInformationHandler = NULL;
	}

	_Ret_maybenull_ CMeshInformationHandler * CMesh::getMeshInformationHandler()
	{
		return m_pMeshInformationHandler.get();
	}

	_Ret_notnull_ CMeshInformationHandler * CMesh::createMeshInformationHandler()
	{
		if (!m_pMeshInformationHandler) 
			m_pMeshInformationHandler = std::make_shared<CMeshInformationHandler>();

		return m_pMeshInformationHandler.get();
	}

}
