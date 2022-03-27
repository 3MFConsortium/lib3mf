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
NMR_ModelReaderNode_Volumetric2201_ScalarField.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ScalarField.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ScalarFieldFromImage3D.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ScalarFieldConstant.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ScalarFieldComposed.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelScalarField.h"
#include "Model/Classes/NMR_ModelScalarFieldFromImage3D.h"
#include "Model/Classes/NMR_ModelScalarFieldConstant.h"
#include "Model/Classes/NMR_ModelScalarFieldComposed.h"
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_ScalarField::CModelReaderNode_Volumetric2201_ScalarField(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
			m_pModel (pModel),
			m_nID (0),
			m_bHasName (false)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric2201_ScalarField::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Parse Content
		parseContent(pXMLReader);

		m_pModel->addResource(m_pScalarField);
	}
	
	
	void CModelReaderNode_Volumetric2201_ScalarField::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELD_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SCALARFIELD_NAME) == 0) {
			if (m_bHasName)
				throw CNMRException(-1); // NMR_ERROR_DUPLICATESCALARFIELDNAME

			m_bHasName = true;
			m_sName = pAttributeName;
		}
	}
	
	void CModelReaderNode_Volumetric2201_ScalarField::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_SCALARFIELDFROMIMAGE3D) == 0)
			{
				PModelScalarFieldFromImage3D pScalarFieldFromImage3D = std::make_shared<CModelScalarFieldFromImage3D>(m_nID, m_pModel, nullptr);
				PModelReaderNode_Volumetric2201_ScalarFieldFromImage3D pXMLNode = std::make_shared<CModelReaderNode_Volumetric2201_ScalarFieldFromImage3D>(m_pModel, pScalarFieldFromImage3D.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (!pScalarFieldFromImage3D->getImage3DResourceID())
				{
					throw CNMRException(-1); // TODO
				}

				m_pScalarField = pScalarFieldFromImage3D;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_SCALARFIELDCONSTANT) == 0)
			{
				PModelScalarFieldConstant pScalarFieldConstant = std::make_shared<CModelScalarFieldConstant>(m_nID, m_pModel);
				PModelReaderNode_Volumetric2201_ScalarFieldConstant pXMLNode = std::make_shared<CModelReaderNode_Volumetric2201_ScalarFieldConstant>(m_pModel, pScalarFieldConstant.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pScalarField = pScalarFieldConstant;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_SCALARFIELDCOMPOSED) == 0)
			{
				PModelReaderNode_Volumetric2201_ScalarFieldComposed pXMLNode = std::make_shared<CModelReaderNode_Volumetric2201_ScalarFieldComposed>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pScalarField = pXMLNode->makeScalarFieldComposed(m_nID);
				if (!m_pScalarField)
					throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
