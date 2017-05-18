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

NMR_OpcPackageWriter.cpp defines an OPC Package writer in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackageWriter.h" 
#include "Common/Platform/NMR_Time.h" 
#include "Common/Platform/NMR_XmlWriter_Native.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {


	COpcPackageWriter::COpcPackageWriter(_In_ PExportStream pExportStream)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pExportStream = pExportStream;
		m_pZIPWriter = std::make_shared<CPortableZIPWriter>(m_pExportStream);
	}

	COpcPackageWriter::~COpcPackageWriter()
	{
		finishPackage();
	}

	POpcPackagePart COpcPackageWriter::addPart(_In_ std::wstring sPath)
	{
		sPath = fnRemoveLeadingPathDelimiter(sPath);
		
		PExportStream pStream = m_pZIPWriter->createEntry(sPath, fnGetUnixTime());
		POpcPackagePart pPart = std::make_shared<COpcPackagePart>(sPath, pStream);
		m_Parts.push_back(pPart);

		return pPart;
	}

	void COpcPackageWriter::addContentType(_In_ std::wstring sExtension, _In_ std::wstring sContentType)
	{
		m_ContentTypes.insert(std::make_pair(sExtension, sContentType));
	}

	POpcPackageRelationship COpcPackageWriter::addRootRelationship(_In_ std::wstring sID, _In_ std::wstring sType, _In_ COpcPackagePart * pTargetPart)
	{
		if (pTargetPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		POpcPackageRelationship pRelationship = std::make_shared<COpcPackageRelationship>(sID, sType, pTargetPart->getURI());
		m_RootRelationships.push_back(pRelationship);
		return pRelationship;
	}

	void COpcPackageWriter::finishPackage()
	{
		writeContentTypes();
		writeRootRelationships();
		auto iIterator = m_Parts.begin();
		while (iIterator != m_Parts.end()) {
			POpcPackagePart pPart = *iIterator;
			if (pPart->hasRelationships()) {
				std::wstring sURI = pPart->getURI();
				std::wstring sName = fnExtractFileName(sURI);
				std::wstring sPath = sURI.substr(0, sURI.length() - sName.length());
				sPath += L"_rels/";
				sPath += sName;
				sPath += std::wstring(L".")+PACKAGE_3D_RELS_EXTENSION;

				PExportStream pStream = m_pZIPWriter->createEntry(sPath, fnGetUnixTime());
				pPart->writeRelationships(pStream);
			}
			iIterator++;
		}

	}

	void COpcPackageWriter::writeContentTypes()
	{
		PExportStream pStream = m_pZIPWriter->createEntry(OPCPACKAGE_PATH_CONTENTTYPES, fnGetUnixTime());
		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pStream);

		pXMLWriter->WriteStartDocument();
		pXMLWriter->WriteStartElement(nullptr, OPC_CONTENTTYPES_CONTAINER, nullptr);
		pXMLWriter->WriteAttributeString(nullptr, L"xmlns", nullptr, OPCPACKAGE_SCHEMA_CONTENTTYPES);

		auto iIterator = m_ContentTypes.begin();
		while (iIterator != m_ContentTypes.end()) {
			pXMLWriter->WriteStartElement(nullptr, OPC_CONTENTTYPES_NODE, nullptr);
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_EXTENSION, nullptr, iIterator->first.c_str());
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_CONTENTTYPE, nullptr, iIterator->second.c_str());
			pXMLWriter->WriteEndElement();

			iIterator++;
		}

		pXMLWriter->WriteFullEndElement();
		pXMLWriter->WriteEndDocument();
		
	}

	void COpcPackageWriter::writeRootRelationships()
	{
		if (m_RootRelationships.size() == 0)
			return;

		PExportStream pStream = m_pZIPWriter->createEntry(OPCPACKAGE_PATH_ROOTRELATIONSHIPS, fnGetUnixTime());
		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pStream);

		pXMLWriter->WriteStartDocument();
		pXMLWriter->WriteStartElement(nullptr, L"Relationships", nullptr);
		pXMLWriter->WriteAttributeString(nullptr, L"xmlns", nullptr, OPCPACKAGE_SCHEMA_RELATIONSHIPS);

		auto iIterator = m_RootRelationships.begin();

		while (iIterator != m_RootRelationships.end()) {
			POpcPackageRelationship pRelationShip = *iIterator;
			pRelationShip->writeToXML(pXMLWriter.get());
			iIterator++;
		}

		pXMLWriter->WriteFullEndElement();
		pXMLWriter->WriteEndDocument();

	}

}
