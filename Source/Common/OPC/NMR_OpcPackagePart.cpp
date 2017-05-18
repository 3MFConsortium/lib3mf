/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_OpcPackagePart.cpp implements an OPC Package Part in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackagePart.h" 
#include "Common/OPC/NMR_OpcPackageRelationship.h"
#include "Common/NMR_Exception.h" 
#include "Common/Platform/NMR_XmlWriter_Native.h" 

namespace NMR {

	COpcPackagePart::COpcPackagePart(_In_ std::wstring sURI, _In_ PExportStream pExportStream)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (sURI.length() == 0)
			throw CNMRException(NMR_ERROR_INVALIDOPCPARTURI);

		m_sURI = sURI;
		m_pExportStream = pExportStream;
	}


	COpcPackagePart::COpcPackagePart(_In_ std::wstring sURI, _In_ PImportStream pImportStream)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (sURI.length() == 0)
			throw CNMRException(NMR_ERROR_INVALIDOPCPARTURI);

		m_sURI = sURI;
		m_pImportStream = pImportStream;
	}


	std::wstring COpcPackagePart::getURI()
	{
		return m_sURI;
	}

	PExportStream COpcPackagePart::getExportStream()
	{
		if (m_pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		return m_pExportStream;
	}

	PImportStream COpcPackagePart::getImportStream()
	{
		if (m_pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		return m_pImportStream;
	}

	POpcPackageRelationship COpcPackagePart::addRelationship(_In_ std::wstring sID, _In_ std::wstring sType, _In_ std::wstring sURI)
	{
		POpcPackageRelationship pRelationship = std::make_shared<COpcPackageRelationship>(sID, sType, sURI);
		for (auto iRelationShip : m_Relationships) {
			if ((iRelationShip->getTargetPartURI() == sURI) && (iRelationShip->getType() == sType) ) {
				throw CNMRException(NMR_ERROR_DUPLICATE_RELATIONSHIP);
			}
			// Include this again for 
			//if (iRelationShip->getType() == sType) {
			//	throw CNMRException(NMR_ERROR_DUPLICATE_RELATIONSHIP);
			//}
		}
		m_Relationships.push_back(pRelationship);
		return pRelationship;
	}

	nfBool COpcPackagePart::hasRelationships()
	{
		return m_Relationships.size() > 0;
	}

	std::list<POpcPackageRelationship> COpcPackagePart::getRelationShips()
	{
		return m_Relationships;
	}

	void COpcPackagePart::writeRelationships(_In_ PExportStream pExportStream)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pExportStream);

		pXMLWriter->WriteStartDocument();
		pXMLWriter->WriteStartElement(nullptr, OPC_RELS_RELATIONSHIP_CONTAINER, nullptr);
		pXMLWriter->WriteAttributeString(nullptr, L"xmlns", nullptr, OPCPACKAGE_SCHEMA_RELATIONSHIPS);

		auto iIterator = m_Relationships.begin();

		while (iIterator != m_Relationships.end()) {
			POpcPackageRelationship pRelationShip = *iIterator;
			pRelationShip->writeToXML(pXMLWriter.get());
			iIterator++;
		}

		pXMLWriter->WriteFullEndElement();
		pXMLWriter->WriteEndDocument();
	}

}
