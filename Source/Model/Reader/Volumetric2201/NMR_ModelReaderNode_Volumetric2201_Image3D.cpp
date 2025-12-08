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
NMR_ModelReaderNode_Volumetric2201_Image3D.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_Image3D.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ImageStack.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_Image3D::CModelReaderNode_Volumetric2201_Image3D(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
			m_pModel (pModel),
			m_modelResourceId (0),
			m_bHasName (false)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric2201_Image3D::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		//if (!m_bHasName)
		//	m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGIMAGE3DNAME), mrwMissingMandatoryValue);

		if (m_modelResourceId == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Parse Content
		parseContent(pXMLReader);

		m_pModel->addResource(m_pImage3D);
	}
	
	
	void CModelReaderNode_Volumetric2201_Image3D::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_ID) == 0) {
			if (m_modelResourceId != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_modelResourceId = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_NAME) == 0) {
			if (m_bHasName)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGE3DNAME);

			m_bHasName = true;
			m_sName = pAttributeName;
		}
	}
	
	void CModelReaderNode_Volumetric2201_Image3D::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_IMAGESTACK) == 0)
			{
				PModelImageStack pImageStack = CModelImageStack::make(m_modelResourceId, m_pModel, 0, 0, 0);

				PModelReaderNode_Volumetric2201_ImageStack pXMLNode = std::make_shared<CModelReaderNode_Volumetric2201_ImageStack>(m_pModel, pImageStack.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pImage3D = pImageStack;
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
