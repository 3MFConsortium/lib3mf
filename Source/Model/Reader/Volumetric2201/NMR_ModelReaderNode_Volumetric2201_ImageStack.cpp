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
NMR_ModelReaderNode_Volumetric2201_ImageStack.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ImageStack.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_ImageSheet.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_ImageStack::CModelReaderNode_Volumetric2201_ImageStack(_In_ CModel* pModel, _In_ CModelImageStack* pImageStack, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_pModel(pModel),
		m_pImageStack(pImageStack),
		m_nRowCount(0),
		m_nColumnCount(0),
		m_nSheetCount(0),
		m_nSheetIndex(0)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_pImageStack == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric2201_ImageStack::parseXML(_In_ CXmlReader* pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (m_nRowCount == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGESTACKSIZE);
		if (m_nColumnCount == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGESTACKSIZE);
		if (m_nSheetCount == 0)
			throw CNMRException(NMR_ERROR_MISSINGIMAGESTACKSHEETCOUNT);

		m_pImageStack->setRowCount(m_nRowCount);
		m_pImageStack->setColumnCount(m_nColumnCount);
		m_pImageStack->setSheetCount(m_nSheetCount);

		m_nSheetIndex = 0;

		// Parse Content
		parseContent(pXMLReader);

		if (m_nSheetCount != m_nSheetIndex)
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDIMAGSHEETCOUNT), mrwMissingMandatoryValue);
	}


	void CModelReaderNode_Volumetric2201_ImageStack::OnAttribute(_In_z_ const nfChar* pAttributeName, _In_z_ const nfChar* pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGESTACK_ROWCOUNT) == 0) {
			if (m_nRowCount != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGESTACKSIZE);

			// Convert to integer and make a input and range check!
			m_nRowCount = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGESTACK_COLUMNCOUNT) == 0) {
			if (m_nColumnCount != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGESTACKSIZE);

			// Convert to integer and make a input and range check!
			m_nColumnCount = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGESTACK_SHEETCOUNT) == 0) {
			if (m_nSheetCount != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEIMAGESTACKSHEETCOUNT);

			// Convert to integer and make a input and range check!
			m_nSheetCount = fnStringToUint32(pAttributeValue);
		}
	}

	void CModelReaderNode_Volumetric2201_ImageStack::OnNSChildElement(_In_z_ const nfChar* pChildName, _In_z_ const nfChar* pNameSpace, _In_ CXmlReader* pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_IMAGESHEET) == 0)
			{
				PModelReaderNode_Volumetric2201_ImageSheet pXMLNode = std::make_shared<CModelReaderNode_Volumetric2201_ImageSheet>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::string sPath = pXMLNode->getPath();
				if (sPath != "") {

					PModelAttachment pAttachment = m_pModel->findModelAttachment(sPath);
					if (pAttachment.get() == nullptr)
						m_pWarnings->addException(CNMRException(NMR_ERROR_IMAGESHEETNOTFOUND), mrwFatal);

					if (m_nSheetIndex >= m_nSheetCount)
						m_pWarnings->addException(CNMRException(NMR_ERROR_TOOMANYIMAGESHEETS), mrwFatal);

					__NMRASSERT(m_pImageStack.get() != nullptr);
					m_pImageStack->setSheet(m_nSheetIndex, pAttachment);
					m_nSheetIndex++;
				}
				else
					m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGIMAGE3DSHEETPATH), mrwMissingMandatoryValue);

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}


}
