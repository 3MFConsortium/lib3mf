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

NMR_OpcPackageWriter.cpp defines an OPC Package writer in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackageWriter.h" 
#include "Common/Platform/NMR_Time.h" 
#include "Common/Platform/NMR_XmlWriter_Native.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include "Model/Classes/NMR_ModelConstants.h"

#include <sstream>

namespace NMR {


	COpcPackageWriter::COpcPackageWriter(_In_ PExportStream pExportStream)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pExportStream = pExportStream;
		m_pZIPWriter = std::make_shared<CPortableZIPWriter>(m_pExportStream, true);

		m_nRelationIDCounter = 0;
	}

	COpcPackageWriter::~COpcPackageWriter()
	{
		finishPackage();
	}

	POpcPackagePart COpcPackageWriter::addPart(_In_ std::string sPath)
	{
		sPath = fnRemoveLeadingPathDelimiter(sPath);
		
		PExportStream pStream = m_pZIPWriter->createEntry(sPath, fnGetUnixTime());
		POpcPackagePart pPart = std::make_shared<COpcPackagePart>(sPath, pStream);
		m_Parts.push_back(pPart);

		return pPart;
	}

	void COpcPackageWriter::addContentType(_In_ std::string sExtension, _In_ std::string sContentType)
	{
		m_DefaultContentTypes.insert(std::make_pair(sExtension, sContentType));
	}

	void COpcPackageWriter::addContentType(POpcPackagePart pOpcPackagePart, std::string sContentType)
	{
		// Follows section 10.1.2.3 of "Ecma Office Open XML Part 2 - Open Packaging Conventions"
		nfBool isOverride = false;

		// Step 1 - Get extension
		std::size_t extensionPosition = pOpcPackagePart->getURI().find_last_of('.');
		if (std::string::npos == extensionPosition) // Step 2 - No extension available, use Override Content Type
			isOverride = true;
		else {
			// Step 3 - Compare extensions with Default Content Types
			std::string extension = pOpcPackagePart->getURI().substr(extensionPosition + 1);
			std::map<std::string, std::string>::iterator defaultSameExtension = m_DefaultContentTypes.find(extension);
			if (m_DefaultContentTypes.end() != defaultSameExtension) {
				// Step 4 - An extension assigned to a Default Content Type matches
				if (defaultSameExtension->second.compare(sContentType) == 0) {
					// Step 4.a - Content Types match, nothing else to do
					isOverride = false;
					addContentType(extension, sContentType);
				}
				else // Step 4.b - Content Types do not match, use Override Content Type
					isOverride = true;
			}
			else // Step 5 - No Default Content Type that matches, use Override Content Type
				isOverride = true;
		}

		if (isOverride)
			m_OverrideContentTypes.insert(std::make_pair(pOpcPackagePart->getURI(), sContentType));
	}

	POpcPackageRelationship COpcPackageWriter::addRootRelationship(_In_ std::string sType, _In_ COpcPackagePart * pTargetPart)
	{
		if (pTargetPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		POpcPackageRelationship pRelationship = std::make_shared<COpcPackageRelationship>(generateRelationShipID(), sType, pTargetPart->getURI());
		m_RootRelationships.push_back(pRelationship);
		return pRelationship;
	}

	POpcPackageRelationship COpcPackageWriter::addPartRelationship(POpcPackagePart pOpcPackagePart, std::string sType, COpcPackagePart * pTargetPart)
	{
		return pOpcPackagePart->addRelationship(generateRelationShipID(), sType, pTargetPart->getURI());
	}

	std::list<POpcPackageRelationship> COpcPackageWriter::addWriterSpecificRelationships(_In_ POpcPackagePart pOpcPackagePart, _In_ COpcPackagePart* pTargetPart)
	{
		return std::list<POpcPackageRelationship>();
	}

	void COpcPackageWriter::finishPackage()
	{
		writeContentTypes();
		writeRootRelationships();
		auto iIterator = m_Parts.begin();
		while (iIterator != m_Parts.end()) {
			POpcPackagePart pPart = *iIterator;
			if (pPart->hasRelationships()) {
				std::string sURI = pPart->getURI();
				std::string sName = fnExtractFileName(sURI);
				std::string sPath = sURI.substr(0, sURI.length() - sName.length());
				sPath += "_rels/";
				sPath += sName;
				sPath += std::string(".")+PACKAGE_3D_RELS_EXTENSION;

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
		pXMLWriter->WriteAttributeString(nullptr, "xmlns", nullptr, OPCPACKAGE_SCHEMA_CONTENTTYPES);

		auto iDefaultIterator = m_DefaultContentTypes.begin();
		while (iDefaultIterator != m_DefaultContentTypes.end()) {
			pXMLWriter->WriteStartElement(nullptr, OPC_CONTENTTYPES_NODE, nullptr);
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_EXTENSION, nullptr, iDefaultIterator->first.c_str());
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_CONTENTTYPE, nullptr, iDefaultIterator->second.c_str());
			pXMLWriter->WriteEndElement();

			iDefaultIterator++;
		}

		auto iOverrideIterator = m_OverrideContentTypes.begin();
		while (iOverrideIterator != m_OverrideContentTypes.end()) {
			pXMLWriter->WriteStartElement(nullptr, OPC_CONTENTTYPES_NODE_OVERRIDE, nullptr);
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_PARTNAME, nullptr, iOverrideIterator->first.c_str());
			pXMLWriter->WriteAttributeString(nullptr, OPC_CONTENTTYPES_ATTRIB_CONTENTTYPE, nullptr, iOverrideIterator->second.c_str());
			pXMLWriter->WriteEndElement();

			iOverrideIterator++;
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
		pXMLWriter->WriteStartElement(nullptr, "Relationships", nullptr);
		pXMLWriter->WriteAttributeString(nullptr, "xmlns", nullptr, OPCPACKAGE_SCHEMA_RELATIONSHIPS);

		auto iIterator = m_RootRelationships.begin();

		while (iIterator != m_RootRelationships.end()) {
			POpcPackageRelationship pRelationShip = *iIterator;
			pRelationShip->writeToXML(pXMLWriter.get());
			iIterator++;
		}

		pXMLWriter->WriteFullEndElement();
		pXMLWriter->WriteEndDocument();

	}

	std::string COpcPackageWriter::generateRelationShipID()
	{
		// Create Unique ID String
		std::stringstream sStream;
		sStream << "rel" << m_nRelationIDCounter;
		m_nRelationIDCounter++;
		return sStream.str();
	}
}
