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
NMR_ModelReaderNode_Volumetric2201_ScalarFieldComposed.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ScalarFieldComposed.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelTexture2D.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_ScalarFieldComposed::CModelReaderNode_Volumetric2201_ScalarFieldComposed(_In_ CModel* pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_pModel(pModel)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric2201_ScalarFieldComposed::parseXML(_In_ CXmlReader* pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}


	void CModelReaderNode_Volumetric2201_ScalarFieldComposed::OnAttribute(_In_z_ const nfChar* pAttributeName, _In_z_ const nfChar* pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD) == 0)
		{
			if (m_bHasMethod)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_eMethod = CModelScalarFieldComposed::methodFromString(pAttributeValue);
			m_bHasMethod = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_SCALARFIELDID1) == 0)
		{
			if (m_bHasScalarFieldId1)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_pScalarFieldId1 = m_pModel->findPackageResourceID(m_pModel->currentPath(), fnStringToUint32(pAttributeValue));
			if (!m_pModel->findScalarField(m_pScalarFieldId1->getUniqueID()))
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
			// check that this is a scalar field
			m_bHasScalarFieldId1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_SCALARFIELDID2) == 0)
		{
			if (m_bHasScalarFieldId2)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_pScalarFieldId2 = m_pModel->findPackageResourceID(m_pModel->currentPath(), fnStringToUint32(pAttributeValue));
			if (!m_pModel->findScalarField(m_pScalarFieldId2->getUniqueID()))
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
			// check that this is a scalar field
			m_bHasScalarFieldId2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_SCALARFIELDMASKID) == 0)
		{
			if (m_bHasScalarFieldIdMask)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_pScalarFieldIdMask = m_pModel->findPackageResourceID(m_pModel->currentPath(), fnStringToUint32(pAttributeValue));
			if (!m_pModel->findScalarField(m_pScalarFieldIdMask->getUniqueID()))
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
			// check that this is a scalar field
			m_bHasScalarFieldIdMask = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_FACTOR1) == 0)
		{
			if (m_bHasFactor1)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_dFactor1 = fnStringToDouble(pAttributeValue);
			m_bHasFactor1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_FACTOR2) == 0)
		{
			if (m_bHasFactor2)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_dFactor2 = fnStringToDouble(pAttributeValue);
			m_bHasFactor2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_TRANSFORM1) == 0)
		{
			if (m_bHasTransform1)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_mTransform1 = fnMATRIX3_fromString(pAttributeValue);
			m_bHasTransform1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_TRANSFORM2) == 0)
		{
			if (m_bHasTransform2)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_mTransform2 = fnMATRIX3_fromString(pAttributeValue);
			m_bHasTransform2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_TRANSFORMMASK) == 0)
		{
			if (m_bHasTransformMask)
				throw CNMRException(NMR_ERROR_DUPLICATE_ATTRIBUTE_SCALARFIELDCOMPOSED);
			m_mTransformMask = fnMATRIX3_fromString(pAttributeValue);
			m_bHasTransformMask = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_Volumetric2201_ScalarFieldComposed::OnNSChildElement(_In_z_ const nfChar* pChildName, _In_z_ const nfChar* pNameSpace, _In_ CXmlReader* pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

	PModelScalarFieldComposed CModelReaderNode_Volumetric2201_ScalarFieldComposed::makeScalarFieldComposed(ModelResourceID nID)
	{
		if (!(m_bHasMethod && m_bHasScalarFieldId1 && m_bHasScalarFieldId2))
		{
			m_pWarnings->addException(CNMRException(NMR_ERROR_MISSING_ATTRIBUTE_SCALARFIELDCOMPOSED), eModelWarningLevel::mrwInvalidMandatoryValue);
			return nullptr;
		}
		if (m_eMethod == eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MASK && !m_bHasScalarFieldIdMask)
		{
			m_pWarnings->addException(CNMRException(NMR_ERROR_MISSING_ATTRIBUTE_SCALARFIELDCOMPOSED), eModelWarningLevel::mrwInvalidMandatoryValue);
			return nullptr;
		}
		PModelScalarFieldComposed pOut = std::make_shared<CModelScalarFieldComposed>(nID, m_pModel);
		pOut->setFactor1(m_dFactor1);
		pOut->setFactor2(m_dFactor2);
		pOut->setMethod(m_eMethod);
		pOut->ScalarFieldReference1()->setFieldReferenceID(m_pScalarFieldId1->getUniqueID());
		pOut->ScalarFieldReference2()->setFieldReferenceID(m_pScalarFieldId2->getUniqueID());
		pOut->ScalarFieldReference1()->setTransform(m_mTransform1);
		pOut->ScalarFieldReference2()->setTransform(m_mTransform2);
		if (m_eMethod == eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MASK)
		{
			pOut->ScalarFieldReferenceMask()->setFieldReferenceID(m_pScalarFieldIdMask->getUniqueID());
			pOut->ScalarFieldReferenceMask()->setTransform(m_mTransformMask);
		}
		return pOut;
	}
}
