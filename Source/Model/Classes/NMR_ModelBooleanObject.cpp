/*++

Copyright (C) 2026 3MF Consortium

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

Minimal in-memory representation of a boolean object.

--*/

#include "Model/Classes/NMR_ModelBooleanObject.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelLevelSetObject.h"
#include "Common/Boolean/NMR_BooleanEngine.h"
#include "Common/NMR_Exception.h"
#include <unordered_set>

namespace NMR {

	namespace {
		nfBool referencesObjectRecursive(_In_ CModelObject * pCandidate, _In_ CModelObject * pTarget, _Inout_ std::unordered_set<CModelObject *> & visited)
		{
			if (pCandidate == nullptr || pTarget == nullptr)
				return false;
			if (pCandidate == pTarget)
				return true;
			if (!visited.insert(pCandidate).second)
				return false;

			auto pBooleanCandidate = dynamic_cast<CModelBooleanObject *>(pCandidate);
			if (pBooleanCandidate == nullptr)
				return false;

			if (referencesObjectRecursive(pBooleanCandidate->getBaseObject(), pTarget, visited))
				return true;

			for (nfUint32 nOperandIndex = 0; nOperandIndex < pBooleanCandidate->getOperandCount(); ++nOperandIndex) {
				auto pOperand = pBooleanCandidate->getOperand(nOperandIndex);
				if (pOperand && referencesObjectRecursive(pOperand->getObject(), pTarget, visited))
					return true;
			}

			return false;
		}

		nfBool createsBooleanReferenceCycle(_In_ CModelObject * pProposedBase, _In_ CModelBooleanObject * pBooleanObject)
		{
			std::unordered_set<CModelObject *> visited;
			return referencesObjectRecursive(pProposedBase, pBooleanObject, visited);
		}

		nfBool isBeamLatticeMeshObject(_In_ CModelObject * pObject)
		{
			auto pMeshObject = dynamic_cast<CModelMeshObject *>(pObject);
			if (pMeshObject == nullptr)
				return false;

			auto pMesh = pMeshObject->getMesh();
			return (pMesh != nullptr) && (pMesh->getBeamCount() > 0);
		}

	}

	CModelBooleanObject::CModelBooleanObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelObject(sID, pModel),
		m_eOperation(eModelBooleanOperation::Union),
		m_bCSGModeEnabled(false),
		m_nExtractionGridResolution(160)
	{
	}

	CModelBooleanObject::~CModelBooleanObject() = default;

	void CModelBooleanObject::setBaseObject(_In_ CModelObject * pObject, _In_ const NMATRIX3 & mTransform)
	{
		if (pObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (dynamic_cast<CModelComponentsObject *>(pObject) != nullptr)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (dynamic_cast<CModelLevelSetObject *>(pObject) != nullptr)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (pObject->getObjectType() != MODELOBJECTTYPE_MODEL)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (isBeamLatticeMeshObject(pObject))
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (createsBooleanReferenceCycle(pObject, this))
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);

		m_pBaseObject = std::make_shared<CModelComponent>(pObject, mTransform);
	}

	CModelObject * CModelBooleanObject::getBaseObject() const
	{
		return m_pBaseObject ? m_pBaseObject->getObject() : nullptr;
	}

	void CModelBooleanObject::setBaseTransform(_In_ const NMATRIX3 & mTransform)
	{
		if (!m_pBaseObject)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);

		m_pBaseObject->setTransform(mTransform);
	}

	NMATRIX3 CModelBooleanObject::getBaseTransform() const
	{
		return m_pBaseObject ? m_pBaseObject->getTransform() : fnMATRIX3_identity();
	}

	void CModelBooleanObject::setOperation(_In_ eModelBooleanOperation eOperation)
	{
		m_eOperation = eOperation;
	}

	eModelBooleanOperation CModelBooleanObject::getOperation() const
	{
		return m_eOperation;
	}

	std::string CModelBooleanObject::getOperationString() const
	{
		switch (m_eOperation) {
		case eModelBooleanOperation::Union:
			return XML_3MF_VALUE_BOOLEAN_OPERATION_UNION;
		case eModelBooleanOperation::Difference:
			return XML_3MF_VALUE_BOOLEAN_OPERATION_DIFFERENCE;
		case eModelBooleanOperation::Intersection:
			return XML_3MF_VALUE_BOOLEAN_OPERATION_INTERSECTION;
		}

		return XML_3MF_VALUE_BOOLEAN_OPERATION_UNION;
	}

	nfBool CModelBooleanObject::setOperationString(_In_ const std::string & sOperation, _In_ nfBool bRaiseException)
	{
		if (sOperation == XML_3MF_VALUE_BOOLEAN_OPERATION_UNION) {
			m_eOperation = eModelBooleanOperation::Union;
			return true;
		}
		if (sOperation == XML_3MF_VALUE_BOOLEAN_OPERATION_DIFFERENCE) {
			m_eOperation = eModelBooleanOperation::Difference;
			return true;
		}
		if (sOperation == XML_3MF_VALUE_BOOLEAN_OPERATION_INTERSECTION) {
			m_eOperation = eModelBooleanOperation::Intersection;
			return true;
		}

		if (bRaiseException)
			throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);

		return false;
	}

	void CModelBooleanObject::setCSGModeEnabled(_In_ nfBool bEnabled)
	{
		m_bCSGModeEnabled = bEnabled;
	}

	nfBool CModelBooleanObject::getCSGModeEnabled() const
	{
		return m_bCSGModeEnabled;
	}

	void CModelBooleanObject::setExtractionGridResolution(_In_ nfUint32 nGridResolution)
	{
		if (nGridResolution == 0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_nExtractionGridResolution = nGridResolution;
	}

	nfUint32 CModelBooleanObject::getExtractionGridResolution() const
	{
		return m_nExtractionGridResolution;
	}

	void CModelBooleanObject::addOperand(_In_ CModelObject * pObject, _In_ const NMATRIX3 & mTransform)
	{
		if (pObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (dynamic_cast<CModelMeshObject *>(pObject) == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (pObject->getObjectType() != MODELOBJECTTYPE_MODEL)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		if (isBeamLatticeMeshObject(pObject))
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);

		m_Operands.push_back(std::make_shared<CModelComponent>(pObject, mTransform));
	}

	nfUint32 CModelBooleanObject::getOperandCount() const
	{
		return (nfUint32)m_Operands.size();
	}

	PModelComponent CModelBooleanObject::getOperand(_In_ nfUint32 nIdx) const
	{
		if (nIdx >= m_Operands.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Operands[nIdx];
	}

	void CModelBooleanObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		if (pMesh == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!m_pBaseObject)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);

		PMesh pWorkingMesh = std::make_shared<CMesh>();
		m_pBaseObject->mergeToMesh(pWorkingMesh.get(), mMatrix);
		std::vector<PMesh> operandMeshes;
		operandMeshes.reserve(m_Operands.size());

		for (const auto & operand : m_Operands) {
			if (!operand)
				throw CNMRException(NMR_ERROR_INVALIDOBJECT);
			auto pOperandMesh = std::make_shared<CMesh>();
			operand->mergeToMesh(pOperandMesh.get(), mMatrix);
			operandMeshes.push_back(pOperandMesh);
		}

		if (m_bCSGModeEnabled) {
			PMesh pCsgMesh = std::make_shared<CMesh>();
			CBooleanEngine::evaluate(pWorkingMesh.get(), operandMeshes, m_eOperation, pCsgMesh.get(), m_nExtractionGridResolution);
			pMesh->mergeMesh(pCsgMesh.get(), fnMATRIX3_identity());
			return;
		}

		// Temporary realization path: flatten referenced meshes with transforms.
		// This preserves object traversal/export behavior until true CSG evaluation is added.
		for (const auto & operandMesh : operandMeshes)
			pWorkingMesh->mergeMesh(operandMesh.get(), fnMATRIX3_identity());

		pMesh->mergeMesh(pWorkingMesh.get(), fnMATRIX3_identity());
	}

	nfBool CModelBooleanObject::isValid()
	{
		if (!m_pBaseObject || m_Operands.empty())
			return false;

		auto pBase = m_pBaseObject->getObject();
		if (!pBase)
			return false;

		if (dynamic_cast<CModelComponentsObject *>(pBase) != nullptr)
			return false;
		if (dynamic_cast<CModelLevelSetObject *>(pBase) != nullptr)
			return false;
		if (pBase->getObjectType() != MODELOBJECTTYPE_MODEL)
			return false;
		if (isBeamLatticeMeshObject(pBase))
			return false;

		for (const auto & operand : m_Operands) {
			if (dynamic_cast<CModelMeshObject *>(operand->getObject()) == nullptr)
				return false;
			if (operand->getObject()->getObjectType() != MODELOBJECTTYPE_MODEL)
				return false;
			if (isBeamLatticeMeshObject(operand->getObject()))
				return false;
		}

		return true;
	}

	void CModelBooleanObject::calculateComponentDepthLevel(nfUint32 nLevel)
	{
		CModelObject::calculateComponentDepthLevel(nLevel);

		if (m_pBaseObject && m_pBaseObject->getObject())
			m_pBaseObject->getObject()->calculateComponentDepthLevel(nLevel + 1);

		for (const auto & operand : m_Operands) {
			if (operand && operand->getObject())
				operand->getObject()->calculateComponentDepthLevel(nLevel + 1);
		}
	}

	nfBool CModelBooleanObject::hasSlices(nfBool bRecursive)
	{
		if (!bRecursive)
			return (this->getSliceStack().get() != nullptr);

		if (this->getSliceStack().get() != nullptr)
			return true;

		if (m_pBaseObject && m_pBaseObject->getObject()->hasSlices(true))
			return true;

		for (const auto & operand : m_Operands) {
			if (operand->getObject()->hasSlices(true))
				return true;
		}

		return false;
	}

	nfBool CModelBooleanObject::isValidForSlices(const NMATRIX3& totalParentMatrix)
	{
		if (this->getSliceStack().get() && !fnMATRIX3_isplanar(totalParentMatrix))
			return false;

		if (m_pBaseObject) {
			auto baseMatrix = fnMATRIX3_multiply(totalParentMatrix, m_pBaseObject->getTransform());
			if (!m_pBaseObject->getObject()->isValidForSlices(baseMatrix))
				return false;
		}

		for (const auto & operand : m_Operands) {
			auto operandMatrix = fnMATRIX3_multiply(totalParentMatrix, operand->getTransform());
			if (!operand->getObject()->isValidForSlices(operandMatrix))
				return false;
		}

		return true;
	}

	void CModelBooleanObject::extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix)
	{
		if (m_pBaseObject) {
			m_pBaseObject->getObject()->extendOutbox(vOutBox, fnMATRIX3_multiply(mAccumulatedMatrix, m_pBaseObject->getTransform()));
		}

		for (const auto & operand : m_Operands) {
			operand->getObject()->extendOutbox(vOutBox, fnMATRIX3_multiply(mAccumulatedMatrix, operand->getTransform()));
		}
	}

	ResourceDependencies CModelBooleanObject::getDependencies()
	{
		ResourceDependencies dependencies;

		if (m_pBaseObject && m_pBaseObject->getObject())
			dependencies.push_back(m_pBaseObject->getObject()->getPackageResourceID());

		for (const auto & operand : m_Operands) {
			if (operand->getObject())
				dependencies.push_back(operand->getObject()->getPackageResourceID());
		}

		return dependencies;
	}
}
