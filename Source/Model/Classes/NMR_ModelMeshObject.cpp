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

NMR_ModelMeshObject.cpp implements the Model Mesh Object Class.
A model mesh object is an in memory representation of the 3MF
mesh object.

--*/

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Common/Math/NMR_PairMatchingTree.h" 

namespace NMR {

	CModelMeshObject::CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelObject(sID, pModel)
	{
		m_pMesh = std::make_shared<CMesh>();
		m_pBeamLatticeAttributes = std::make_shared<CModelMeshBeamLatticeAttributes>();
	}

	CModelMeshObject::CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PMesh pMesh)
		: CModelObject(sID, pModel)
	{
		m_pMesh = pMesh;
		if (m_pMesh.get() == nullptr)
			m_pMesh = std::make_shared<CMesh>();
		m_pBeamLatticeAttributes = std::make_shared<CModelMeshBeamLatticeAttributes>();
	}

	CModelMeshObject::~CModelMeshObject()
	{
		m_pMesh = NULL;
	}

	_Ret_notnull_ CMesh * CModelMeshObject::getMesh()
	{
		return m_pMesh.get();
	}

	void CModelMeshObject::setMesh(_In_ PMesh pMesh)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pMesh = pMesh;
	}

	void CModelMeshObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		__NMRASSERT(pMesh);
		pMesh->mergeMesh(m_pMesh.get(), mMatrix);
	}

	void CModelMeshObject::setObjectType(_In_ eModelObjectType ObjectType)
	{
		if ((ObjectType != MODELOBJECTTYPE_MODEL) && (ObjectType != MODELOBJECTTYPE_SOLIDSUPPORT)) {
			if (m_pMesh->getBeamCount() > 0)
				throw CNMRException(NMR_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE);
		}
		CModelObject::setObjectType(ObjectType);
	}

	nfBool CModelMeshObject::isValid()
	{
		eModelObjectType eType = getObjectType();
		switch (eType) {
		case MODELOBJECTTYPE_MODEL: return isManifoldAndOriented();
		case MODELOBJECTTYPE_SUPPORT: return true;
		case MODELOBJECTTYPE_SOLIDSUPPORT: return isManifoldAndOriented();
		case MODELOBJECTTYPE_SURFACE: return true;
		default:
			return false;

		}
	}

	nfBool CModelMeshObject::isValidForSlices(const NMATRIX3& totalParentMatrix)
	{
		if (!this->hasSlices()) {
			return true;
		}
		else {
			return fnMATRIX3_isplanar(totalParentMatrix);
		}
	}

	nfBool CModelMeshObject::hasSlices()
	{
		return (this->getSliceStackId() != 0);
	}

	nfBool CModelMeshObject::isManifoldAndOriented()
	{
		if (!m_pMesh->checkSanity())
			return false;

		nfUint32 nNodeCount = m_pMesh->getNodeCount();
		nfUint32 nFaceCount = m_pMesh->getFaceCount();

		if (nNodeCount < 3)
			return false;
		if (nFaceCount < 3)
			return false;

		CPairMatchingTree PairMatchingTree;
		nfUint32 nFaceIndex;
		nfInt32 nEdgeIndex;
		nfInt32 nEdgeCounter = 0;
		nfUint32 j;

		// Build Edge Tree
		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			MESHFACE * pFace = m_pMesh->getFace(nFaceIndex);

			for (j = 0; j < 3; j++) {
				nfInt32 nNodeIndex1 = pFace->m_nodeindices[j];
				nfInt32 nNodeIndex2 = pFace->m_nodeindices[(j + 1) % 3];

				if (!PairMatchingTree.checkMatch(nNodeIndex1, nNodeIndex2, nEdgeIndex)) {
					PairMatchingTree.addMatch(nNodeIndex1, nNodeIndex2, nEdgeCounter);
					nEdgeCounter++;

					if (nEdgeCounter > NMR_MESH_MAXEDGECOUNT)
						throw CNMRException(NMR_ERROR_INVALIDEDGEINDEX);
				}
			}

		}

		// Count positively and negatively oriented edges...
		std::vector<nfInt32> PositiveOrientations;
		std::vector<nfInt32> NegativeOrientations;

		// Create Edge arrays
		PositiveOrientations.resize(nEdgeCounter);
		NegativeOrientations.resize(nEdgeCounter);
		for (nEdgeIndex = 0; nEdgeIndex < nEdgeCounter; nEdgeIndex++) {
			PositiveOrientations[nEdgeIndex] = 0;
			NegativeOrientations[nEdgeIndex] = 0;
		}

		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			MESHFACE * pFace = m_pMesh->getFace(nFaceIndex);

			for (j = 0; j < 3; j++) {
				nfInt32 nNodeIndex1 = pFace->m_nodeindices[j];
				nfInt32 nNodeIndex2 = pFace->m_nodeindices[(j + 1) % 3];

				if (PairMatchingTree.checkMatch(nNodeIndex1, nNodeIndex2, nEdgeIndex)) {
					if ((nEdgeIndex < 0) || (nEdgeIndex >= nEdgeCounter))
						throw CNMRException(NMR_ERROR_INVALIDEDGEINDEX);

					if (nNodeIndex1 <= nNodeIndex2) {
						PositiveOrientations[nEdgeIndex]++;
					}
					else {
						NegativeOrientations[nEdgeIndex]++;
					}
				}
				else {
					throw CNMRException(NMR_ERROR_INVALIDMESHTOPOLOGY);
				}
			}
		}

		// Check Edge Orientations and determine manifoldness
		for (nEdgeIndex = 0; nEdgeIndex < nEdgeCounter; nEdgeIndex++) {
			if (PositiveOrientations[nEdgeIndex] != 1)
				return false;
			if (NegativeOrientations[nEdgeIndex] != 1)
				return false;
		}

		// Mesh is non-empty, oriented and manifold
		return true;
	}


	_Ret_notnull_ CModelMeshBeamLatticeAttributes * CModelMeshObject::getBeamLatticeAttributes()
	{
		return m_pBeamLatticeAttributes.get();
	}

	void CModelMeshObject::setBeamLatticeAttributes(_In_ PModelMeshBeamLatticeAttributes pBeamLatticeAttributes)
	{
		if (!pBeamLatticeAttributes)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pBeamLatticeAttributes = pBeamLatticeAttributes;
	}
}


