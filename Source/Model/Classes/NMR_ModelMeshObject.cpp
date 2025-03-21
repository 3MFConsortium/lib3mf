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

NMR_ModelMeshObject.cpp implements the Model Mesh Object Class.
A model mesh object is an in memory representation of the 3MF
mesh object.

--*/

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Common/Math/NMR_PairMatchingTree.h" 

#define TRIANGLESET_MAXCOUNT (1024 * 1024 * 1024)

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

	nfBool CModelMeshObject::hasSlices(nfBool bRecursive)
	{
		return (this->getSliceStack().get() != nullptr);
	}

	nfBool CModelMeshObject::isValidForSlices(const NMATRIX3& totalParentMatrix)
	{
		if (!this->getSliceStack().get()) {
			return true;
		}
		else {
			return fnMATRIX3_isplanar(totalParentMatrix);
		}
	}

	nfBool CModelMeshObject::isValidForBeamLattices()
	{
		return ( (getObjectType() == eModelObjectType::MODELOBJECTTYPE_MODEL) || (getObjectType() == eModelObjectType::MODELOBJECTTYPE_SOLIDSUPPORT) );
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


	_Ret_notnull_ PModelMeshBeamLatticeAttributes CModelMeshObject::getBeamLatticeAttributes()
	{
		return m_pBeamLatticeAttributes;
	}

	void CModelMeshObject::setBeamLatticeAttributes(_In_ PModelMeshBeamLatticeAttributes pBeamLatticeAttributes)
	{
		if (!pBeamLatticeAttributes)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pBeamLatticeAttributes = pBeamLatticeAttributes;
	}

	PModelVolumeData CModelMeshObject::getVolumeData()
	{
		return m_pVolumeData;
	}

	void CModelMeshObject::setVolumeData(_In_ PModelVolumeData pVolumeData)
	{
		if (!pVolumeData)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pVolumeData = pVolumeData;
	}

	void CModelMeshObject::extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix)
	{
		m_pMesh->extendOutbox(vOutBox, mAccumulatedMatrix);
	}

	ResourceDependencies CModelMeshObject::getDependencies()
	{
		ResourceDependencies dependencies;
		if(m_pVolumeData)
		{
			dependencies.push_back(
				m_pVolumeData->getPackageResourceID());
		}

		return dependencies;
	}

	void CModelMeshObject::deleteTriangleSet(_In_ CModelTriangleSet* pTriangleSet)
	{
		if (pTriangleSet != nullptr) {
			auto sIdentifier = pTriangleSet->getIdentifier();
			m_TriangleSets.erase(std::remove_if(
				m_TriangleSets.begin(),
				m_TriangleSets.end(),
				[sIdentifier](PModelTriangleSet const& pSet) { return (pSet->getIdentifier() == sIdentifier); }
			));
			
			m_TriangleSetMap.erase(sIdentifier);
	
		}
	}

	PModelTriangleSet CModelMeshObject::findTriangleSet(const std::string& sIdentifier)
	{
		auto iIterator = m_TriangleSetMap.find(sIdentifier);
		if (iIterator == m_TriangleSetMap.end())
			return nullptr;

		return iIterator->second;

	}

	PModelTriangleSet CModelMeshObject::addTriangleSet(const std::string& sIdentifier, const std::string& sName)
	{
		auto iIterator = m_TriangleSetMap.find(sIdentifier);
		if (iIterator != m_TriangleSetMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATETRIANGLESET);

		if (m_TriangleSets.size() >= TRIANGLESET_MAXCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYTRIANGLESETS);

		auto pTriangleSet = std::make_shared<CModelTriangleSet>(sName, sIdentifier);
		m_TriangleSets.push_back(pTriangleSet);
		m_TriangleSetMap.insert(std::make_pair (sIdentifier, pTriangleSet));

		return pTriangleSet;

	}

	PModelTriangleSet CModelMeshObject::addTriangleSet(PModelTriangleSet pModelTriangleSet)
	{
		if (pModelTriangleSet.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto iIterator = m_TriangleSetMap.find(pModelTriangleSet->getIdentifier());
		if (iIterator != m_TriangleSetMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATETRIANGLESET);

		if (m_TriangleSets.size() >= TRIANGLESET_MAXCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYTRIANGLESETS);

		m_TriangleSets.push_back(pModelTriangleSet);
		m_TriangleSetMap.insert(std::make_pair(pModelTriangleSet->getIdentifier(), pModelTriangleSet));

		return pModelTriangleSet;
	}

	uint32_t CModelMeshObject::getTriangleSetCount()
	{
		return (uint32_t)m_TriangleSets.size();
	}

	PModelTriangleSet CModelMeshObject::getTriangleSet(const uint32_t nIndex)
	{
		if (nIndex >= m_TriangleSets.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_TriangleSets[nIndex];

	}

}


