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
#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"
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

		nfInt32 nIdx, nNodeCount, nFaceCount, nBeamCount, nBallCount, j;
		MESHNODE * pNode;
		MESHFACE * pFace;
		MESHBEAM * pBeam;
		MESHBALL * pBall;
		MESHNODE * pFaceNodes[3];
		MESHNODE * pBeamNodes[2];
		MESHNODE * pBallNode;

		// Copy Mesh Information
		CMeshInformationHandler * pOtherMeshInformationHandler = pMesh->getMeshInformationHandler();
		if (pOtherMeshInformationHandler) {
			createMeshInformationHandler();
			m_pMeshInformationHandler->addInfoTableFrom(pOtherMeshInformationHandler, getFaceCount ());
		}

		nNodeCount = pMesh->getNodeCount();
		nFaceCount = pMesh->getFaceCount();
		nBeamCount = pMesh->getBeamCount();
		nBallCount = pMesh->getBallCount();

		if (nNodeCount > 0) {
			std::vector<MESHNODE *> aNewNodes;
			aNewNodes.resize(nNodeCount);

			for (nIdx = 0; nIdx < nNodeCount; nIdx++) {
				pNode = pMesh->getNode(nIdx);
				NVEC3 vPosition = fnMATRIX3_apply(mMatrix, pNode->m_position);
				aNewNodes[nIdx] = addNode(vPosition);
			}

			if (nFaceCount > 0) {
				if (m_pMeshInformationHandler && pOtherMeshInformationHandler) {
					m_pMeshInformationHandler->cloneDefaultInfosFrom(pOtherMeshInformationHandler);
				}

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
					addBeam(pBeamNodes[0], pBeamNodes[1], pBeam->m_radius[0], pBeam->m_radius[1], pBeam->m_capMode[0], pBeam->m_capMode[1]);
				}
			}
			if (nBallCount > 0) {
				for (nIdx = 0; nIdx < nBallCount; nIdx++) {
					pBall = pMesh->getBall(nIdx);
					if ((pBall->m_nodeindex < 0) || (pBall->m_nodeindex >= nNodeCount))
						throw CNMRException(NMR_ERROR_INVALIDNODEINDEX);

					pBallNode = aNewNodes[pBall->m_nodeindex];
					addBall(pBallNode, pBall->m_radius);
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
		if (nNodeCount >= NMR_MESH_MAXNODECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYNODES);

		// Allocate Data
		nfUint32 nNewIndex;
		pNode = m_Nodes.allocData(nNewIndex);
		pNode->m_index = nNewIndex;
		pNode->m_position = vPosition;

		return pNode;
	}
	_Ret_notnull_ MESHNODE * CMesh::addNode(_In_ const nfFloat posX, _In_ const nfFloat posY, _In_ const nfFloat posZ)
	{
		MESHNODE * pNode;

		// Check Position Validity
		if (fabs(posX) > NMR_MESH_MAXCOORDINATE)
			throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);
		if (fabs(posY) > NMR_MESH_MAXCOORDINATE)
			throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);
		if (fabs(posZ) > NMR_MESH_MAXCOORDINATE)
			throw CNMRException(NMR_ERROR_INVALIDCOORDINATES);

		// Check Node Quota
		nfUint32 nNodeCount = getNodeCount();
		if (nNodeCount >= NMR_MESH_MAXNODECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYNODES);

		// Allocate Data
		nfUint32 nNewIndex;
		pNode = m_Nodes.allocData(nNewIndex);
		pNode->m_index = nNewIndex;
		pNode->m_position.m_values.x = posX;
		pNode->m_position.m_values.y = posY;
		pNode->m_position.m_values.z = posZ;

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

		if (nFaceCount >= NMR_MESH_MAXFACECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYFACES);

		nfUint32 nNewIndex = 0;

		pFace = m_Faces.allocData (nNewIndex);
		pFace->m_nodeindices[0] = pNode1->m_index;
		pFace->m_nodeindices[1] = pNode2->m_index;
		pFace->m_nodeindices[2] = pNode3->m_index;
		pFace->m_index = nNewIndex;

		if (m_pMeshInformationHandler)
			m_pMeshInformationHandler->addFace(getFaceCount());

		return pFace;
	}
	
	_Ret_notnull_ MESHFACE * CMesh::addFace(_In_ nfInt32 nNodeIndex1, _In_ nfInt32 nNodeIndex2, _In_ nfInt32 nNodeIndex3)
	{
		if ((nNodeIndex1 == nNodeIndex2) || (nNodeIndex1 == nNodeIndex3) || (nNodeIndex2 == nNodeIndex3))
			throw CNMRException(NMR_ERROR_DUPLICATENODE);

		MESHFACE * pFace;
		nfUint32 nFaceCount = getFaceCount();

		if (nFaceCount >= NMR_MESH_MAXFACECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYFACES);

		nfUint32 nNewIndex;

		pFace = m_Faces.allocData(nNewIndex);
		pFace->m_nodeindices[0] = nNodeIndex1;
		pFace->m_nodeindices[1] = nNodeIndex2;
		pFace->m_nodeindices[2] = nNodeIndex3;
		pFace->m_index = nNewIndex;

		if (m_pMeshInformationHandler)
			m_pMeshInformationHandler->addFace(getFaceCount());

		return pFace;
	}

	_Ret_notnull_ MESHBEAM * CMesh::addBeam(_In_ MESHNODE * pNode1, _In_ MESHNODE * pNode2,
		_In_ nfDouble dRadius1, _In_ nfDouble dRadius2,
		_In_ nfInt32 eCapMode1, _In_ nfInt32 eCapMode2)
	{
		if ((!pNode1) || (!pNode2))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (pNode1 == pNode2)
			throw CNMRException(NMR_ERROR_DUPLICATENODE);

		MESHBEAM * pBeam;
		nfUint32 nBeamCount = getBeamCount();

		if (nBeamCount >= NMR_MESH_MAXBEAMCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYBEAMS);

		nfUint32 nNewIndex;

		pBeam = m_BeamLattice.m_Beams.allocData(nNewIndex);
		pBeam->m_nodeindices[0] = pNode1->m_index;
		pBeam->m_nodeindices[1] = pNode2->m_index;
		pBeam->m_index = nNewIndex;
		pBeam->m_radius[0] = dRadius1;
		pBeam->m_radius[1] = dRadius2;

		pBeam->m_capMode[0] = eCapMode1;
		pBeam->m_capMode[1] = eCapMode2;

		m_BeamLattice.m_OccupiedNodes.insert({ pNode1->m_index, pNode2->m_index });

		return pBeam;
	}

	PBEAMSET CMesh::addBeamSet()
	{
		m_BeamLattice.m_pBeamSets.push_back(std::make_shared<BEAMSET>());
		return m_BeamLattice.m_pBeamSets.back();
	}

	_Ret_notnull_ MESHBALL * CMesh::addBall(_In_ MESHNODE * pNode, _In_ nfDouble dRadius)
	{
		if ((!pNode))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		MESHBALL * pBall;
		nfUint32 nBallCount = getBallCount();

		if (nBallCount >= NMR_MESH_MAXBALLCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYBALLS);

		// Ensure that at least one beam exists at this node
		if (m_BeamLattice.m_OccupiedNodes.find(pNode->m_index) == m_BeamLattice.m_OccupiedNodes.end()) {
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}

		nfUint32 nNewIndex;

		pBall = m_BeamLattice.m_Balls.allocData(nNewIndex);
		pBall->m_nodeindex = pNode->m_index;
		pBall->m_index = nNewIndex;
		pBall->m_radius = dRadius;

		return pBall;
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

	nfUint32 CMesh::getBallCount()
	{
		return m_BeamLattice.m_Balls.getCount();
	}

	nfUint32 CMesh::getOccupiedNodeCount()
	{
		return (nfUint32)m_BeamLattice.m_OccupiedNodes.size();
	}

	nfBool CMesh::isNodeOccupied(_In_ nfUint32 nIdx)
	{
		return m_BeamLattice.m_OccupiedNodes.find(nIdx) != m_BeamLattice.m_OccupiedNodes.end();
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

	_Ret_notnull_ MESHBALL * CMesh::getBall(_In_ nfUint32 nIdx)
	{
		return m_BeamLattice.m_Balls.getData(nIdx);
	}

	_Ret_notnull_ MESHNODE * CMesh::getOccupiedNode(_In_ nfUint32 nIdx)
	{
		std::unordered_set<nfInt32>::iterator iter = m_BeamLattice.m_OccupiedNodes.begin();
		std::advance(iter, nIdx);
		return getNode(*iter);
	}

	void CMesh::setBeamLatticeMinLength(nfDouble dMinLength)
	{
		m_BeamLattice.m_dMinLength = dMinLength;
	}

	nfDouble CMesh::getBeamLatticeMinLength()
	{
		return m_BeamLattice.m_dMinLength;
	}

	nfDouble CMesh::getDefaultBeamRadius()
	{
		return 1.0;
	}

	void CMesh::setDefaultBallRadius(nfDouble dDefaultBallRadius)
	{
		m_BeamLattice.m_dDefaultBallRadius = dDefaultBallRadius;
	}

	nfDouble CMesh::getDefaultBallRadius()
	{
		return m_BeamLattice.m_dDefaultBallRadius;
	}

	nfBool CMesh::getBeamLatticeAccuracy(nfDouble& dAccuracy)
	{
		dAccuracy = 1.0;
		return false;
	}

	void CMesh::setBeamLatticeBallMode(eModelBeamLatticeBallMode eBallMode)
	{
		m_BeamLattice.m_eBallMode = eBallMode;
	}

	eModelBeamLatticeBallMode CMesh::getBeamLatticeBallMode()
	{
		return m_BeamLattice.m_eBallMode;
	}

	eModelBeamLatticeCapMode CMesh::getBeamLatticeCapMode()
	{
		return eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
	}

	nfBool CMesh::checkSanity()
	{
		nfUint32 nIdx, j;

		nfUint32 nNodeCount = getNodeCount();
		nfUint32 nFaceCount = getFaceCount();
		nfUint32 nBeamCount = getBeamCount();
		nfUint32 nBallCount = getBallCount();

		// max 2^31-1 billion Nodes/Faces
		if (nNodeCount > NMR_MESH_MAXNODECOUNT)
			return false;
		if (nFaceCount > NMR_MESH_MAXFACECOUNT)
			return false;
		if (nBeamCount > NMR_MESH_MAXBEAMCOUNT)
			return false;
		if (nBallCount > NMR_MESH_MAXBALLCOUNT)
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

		for (nIdx = 0; nIdx < nBallCount; nIdx++) {
			MESHBALL* ball = getBall(nIdx);
			if ((ball->m_nodeindex < 0) || (((nfUint32)ball->m_nodeindex) >= nNodeCount))
				return false;
		}
		return true;
	}

	void CMesh::clear()
	{
		m_pMeshInformationHandler.reset();
		m_Faces.clearAllData();
		m_Nodes.clearAllData();
		clearBeamLattice();
	}
	
	void CMesh::clearBeamLattice() {
		m_BeamLattice.clear();
	}

	void CMesh::clearBeamLatticeBeams() {
		m_BeamLattice.clearBeams();
	}

	void CMesh::clearBeamLatticeBalls() {
		m_BeamLattice.clearBalls();
	}

	void CMesh::scanOccupiedNodes() {
		m_BeamLattice.m_OccupiedNodes.clear();

		nfUint32 beamCount = m_BeamLattice.m_Beams.getCount();
		for (nfUint32 iBeam = 0; iBeam < beamCount; iBeam++) {
			MESHBEAM * meshBeam = getBeam(iBeam);
			m_BeamLattice.m_OccupiedNodes.insert({ meshBeam->m_nodeindices[0], meshBeam->m_nodeindices[1] });
		}
	}

	void CMesh::validateBeamLatticeBalls() {
		nfUint32 ballCount = m_BeamLattice.m_Balls.getCount();
		MESHBALL * ballData = new MESHBALL[ballCount];
		for (nfUint32 iBall = 0; iBall < ballCount; iBall++) {
			ballData[iBall] = *m_BeamLattice.m_Balls.getData(iBall);
		}

		clearBeamLatticeBalls();
		for (nfUint32 iBall = 0; iBall < ballCount; iBall++) {
			try {
				addBall(getNode(ballData[iBall].m_nodeindex), ballData[iBall].m_radius);
			}
			catch (CNMRException) {}
		}

		delete [] ballData;
	}

	void CMesh::clearMeshInformationHandler()
	{
		m_pMeshInformationHandler.reset();
	}

	void CMesh::patchMeshInformationResources(_In_ std::map<UniqueResourceID, UniqueResourceID> &oldToNewMapping)
	{
		NMR::CMeshInformationHandler *pMeshInformationHandler = this->getMeshInformationHandler();
		if (pMeshInformationHandler) {
			NMR::CMeshInformation *pProperties = dynamic_cast<NMR::CMeshInformation_Properties *>(pMeshInformationHandler->getInformationByType(0, NMR::emiProperties));
			if (pProperties) {
				NMR::MESHINFORMATION_PROPERTIES * pDefaultData = (NMR::MESHINFORMATION_PROPERTIES*)pProperties->getDefaultData();
				if (pDefaultData && pDefaultData->m_nUniqueResourceID != 0) {
					NMR::UniqueResourceID nNewResourceID = oldToNewMapping[pDefaultData->m_nUniqueResourceID];
					if (nNewResourceID == 0)
						throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
					pDefaultData->m_nUniqueResourceID = nNewResourceID;
				}
				for (NMR::nfUint32 nFaceIndex = 0; nFaceIndex < this->getFaceCount(); nFaceIndex++) {
					NMR::MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(nFaceIndex);
					if (pFaceData && pFaceData->m_nUniqueResourceID != 0) {
						NMR::UniqueResourceID nNewResourceID = oldToNewMapping[pFaceData->m_nUniqueResourceID];
						if (nNewResourceID == 0)
							throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
						pFaceData->m_nUniqueResourceID = nNewResourceID;
					}
				}
			}
		}
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

	void CMesh::extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix)
	{
		if (fnMATRIX3_isIdentity(mAccumulatedMatrix)) {
			for (nfUint32 iNode = 0; iNode < getNodeCount(); iNode++) {
				fnOutboxMergeVector(vOutBox, getNode(iNode)->m_position);
			}
		}
		else {
			for (nfUint32 iNode = 0; iNode < getNodeCount(); iNode++) {
				fnOutboxMergeVector(vOutBox, fnMATRIX3_apply(mAccumulatedMatrix, getNode(iNode)->m_position));
			}
		}
	}
}
