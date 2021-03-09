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

NMR_ModelReaderNode100_MetaDataGroup.cpp implements the Model Reader MetaDataGroup
Node Class. A metadatagroup reader node is a parser for the metadatagroup node
of an XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaDataGroup.h"

#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelReaderNode100_MetaDataGroup::CModelReaderNode100_MetaDataGroup(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_MetaDataGroup = std::make_shared<CModelMetaDataGroup>();
	}

	void CModelReaderNode100_MetaDataGroup::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_MetaDataGroup::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {

			if (strcmp(pChildName, XML_3MF_ELEMENT_METADATA) == 0) {
				PModelReaderNode100_MetaData pXMLNode = std::make_shared<CModelReaderNode100_MetaData>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::string sKey = pXMLNode->getKey();
				std::string sValue = pXMLNode->getValue();
				std::string sType = pXMLNode->getType();
				nfBool bPreserve = pXMLNode->getPreserve();

				if (!sKey.empty()) {
					if (m_MetaDataGroup->hasMetaData(sKey)) {
						m_pWarnings->addWarning(NMR_ERROR_DUPLICATEMETADATA, mrwInvalidOptionalValue);
					}
					else {
						std::string sNameSpace, sName;
						decomposeKeyIntoNamespaceAndName(sKey, sNameSpace, sName);
						if (!sNameSpace.empty()) {
							std::string sNameSpaceURI;
							if (!pXMLReader->GetNamespaceURI(sNameSpace, sNameSpaceURI)) {
								m_pWarnings->addException(CNMRException(NMR_ERROR_METADATA_COULDNOTGETNAMESPACE), mrwInvalidOptionalValue);
								sNameSpaceURI = sNameSpace;
							}
							m_MetaDataGroup->addMetaData(sNameSpaceURI, sName, sValue, sType, bPreserve);
						}
						else {
							// default namespace
							if (CModelMetaData::isValidNamespaceAndName("", sName)) {
								m_MetaDataGroup->addMetaData("", sName, sValue, sType, bPreserve);
							}
							else
								m_pWarnings->addException(CNMRException(NMR_ERROR_UNKNOWNMETADATA), mrwInvalidOptionalValue);
						}
					}
				}
				else {
					m_pWarnings->addWarning(NMR_ERROR_INVALIDMETADATA, mrwInvalidOptionalValue);
				}
			}
		}
	}

	void CModelReaderNode100_MetaDataGroup::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

	}

	void CModelReaderNode100_MetaDataGroup::OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pText);
	}

	PModelMetaDataGroup CModelReaderNode100_MetaDataGroup::getMetaDataGroup()
	{
		return m_MetaDataGroup;
	}

}
