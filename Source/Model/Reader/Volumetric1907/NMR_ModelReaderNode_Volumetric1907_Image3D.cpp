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
NMR_ModelReaderNode_Volumetric1907_Image3D.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Image3D.h"
#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Image3DSheet.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_Image3D::CModelReaderNode_Volumetric1907_Image3D(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
			m_pModel (pModel),
			m_nID (0),
			m_bHasName (false),
			m_nSizeX (0),
			m_nSizeY (0),
			m_nSheetCount (0),
			m_nSheetIndex (0)


	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric1907_Image3D::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasName)
			m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGIMAGE3DNAME), mrwMissingMandatoryValue);

		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);
		if (m_nSizeX == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGE3DSIZE);
		if (m_nSizeY == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGE3DSIZE);
		if (m_nSheetCount == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGE3DSHEETCOUNT);

		m_pImage3D = CModelImage3D::make(m_nID, m_pModel, m_nSizeX, m_nSizeY, m_nSheetCount);
		m_nSheetIndex = 0;

		// Parse Content
		parseContent(pXMLReader);

		m_pModel->addResource(m_pImage3D);

		if (m_nSheetCount != m_nSheetIndex)
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDIMAGE3DSHEETCOUNT), mrwMissingMandatoryValue);
	}
	
	
	void CModelReaderNode_Volumetric1907_Image3D::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_NAME) == 0) {
			if (m_bHasName)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGE3DNAME);

			m_bHasName = true;
			m_sName = pAttributeName;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_SIZEX) == 0) {
			if (m_nSizeX != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGE3DSIZE);

			// Convert to integer and make a input and range check!
			m_nSizeX = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_SIZEY) == 0) {
			if (m_nSizeY != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGE3DSIZE);

			// Convert to integer and make a input and range check!
			m_nSizeY = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3D_SHEETCOUNT) == 0) {
			if (m_nSheetCount != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGE3DSHEETCOUNT);

			// Convert to integer and make a input and range check!
			m_nSheetCount = fnStringToUint32(pAttributeValue);
		}
	}
	
	

	void CModelReaderNode_Volumetric1907_Image3D::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_IMAGE3DSHEET) == 0)
			{
				PModelReaderNode_Volumetric1907_Image3DSheet pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_Image3DSheet>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::string sPath = pXMLNode->getPath();
				if (sPath != "") {

					PModelAttachment pAttachment = m_pModel->findModelAttachment(sPath);
					if (pAttachment.get() == nullptr)
						m_pWarnings->addException(CNMRException(NMR_ERROR_IMAGE3DSHEETNOTFOUND), mrwFatal);

					if (m_nSheetIndex >= m_nSheetCount)
						m_pWarnings->addException(CNMRException(NMR_ERROR_TOOMANYIMAGE3DSHEETS), mrwFatal);

					__NMRASSERT(m_pImage3D.get() != nullptr);
					m_pImage3D->setSheet(m_nSheetIndex, pAttachment);
					m_nSheetIndex++;


				} else
					m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGIMAGE3DSHEETPATH), mrwMissingMandatoryValue);

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}


}
