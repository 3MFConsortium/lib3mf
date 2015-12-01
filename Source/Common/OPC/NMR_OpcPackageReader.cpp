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

NMR_OpcPackageReader.cpp defines an OPC Package reader in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackageReader.h" 
#include "Common/OPC/NMR_OpcPackageRelationshipReader.h" 
#include "Common/Platform/NMR_ImportStream_ZIP.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include <iostream>

namespace NMR {


	COpcPackageReader::COpcPackageReader(_In_ PImportStream pImportStream)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_ZIPError.str = nullptr;
		m_ZIPError.sys_err = 0;
		m_ZIPError.zip_err = 0;
		m_ZIParchive = nullptr;
		m_ZIPsource = nullptr;

		try {
			// determine stream size
			nfUint64 nStreamSize = pImportStream->retrieveSize();
			pImportStream->seekPosition(0, true);

			if (nStreamSize == 0)
				throw CNMRException(NMR_ERROR_COULDNOTGETSTREAMPOSITION);

			// read ZIP into memory
			m_Buffer.resize ((size_t) nStreamSize);
			pImportStream->readBuffer(&m_Buffer[0], nStreamSize, true);

			// create ZIP objects
			zip_error_init(&m_ZIPError);
			m_ZIPsource = zip_source_buffer_create(&m_Buffer[0], (size_t) nStreamSize, 0, &m_ZIPError);
			if (m_ZIPsource == nullptr)
				throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);

			m_ZIParchive = zip_open_from_source(m_ZIPsource, ZIP_RDONLY | ZIP_CHECKCONS, &m_ZIPError);
			if (m_ZIParchive == nullptr)
				throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);

			// Get ZIP Content
			nfInt64 nEntryCount = zip_get_num_entries(m_ZIParchive, ZIP_FL_UNCHANGED);
			if (nEntryCount < 0)
				throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);

			// List Entries
			nfInt64 nIndex;
			for (nIndex = 0; nIndex < nEntryCount; nIndex++) {
				const char * pszName = zip_get_name(m_ZIParchive, (nfUint64) nIndex, ZIP_FL_ENC_GUESS);
				std::string sUTF8Name(pszName);
				std::wstring sUTF16Name = fnUTF8toUTF16(sUTF8Name);
				m_ZIPEntries.insert(std::make_pair(sUTF16Name, nIndex));
			}

			readContentTypes();
			readRootRelationships();

		}
		catch (...)
		{
			releaseZIP();
			throw;
		}
	}

	COpcPackageReader::~COpcPackageReader()
	{
		releaseZIP();
	}

	_Ret_maybenull_ COpcPackageRelationship * COpcPackageReader::findRootRelation(_In_ std::wstring sRelationType, _In_ nfBool bMustBeUnique)
	{
		COpcPackageRelationship * pResultRelationship = nullptr;
		auto iIterator = m_RootRelationships.begin();
		while (iIterator != m_RootRelationships.end()) {
			POpcPackageRelationship pRelationship = *iIterator;

			std::wstring sType = pRelationship->getType();
			if (sType == sRelationType) {
				if (pResultRelationship == nullptr) {
					pResultRelationship = pRelationship.get();
				}
				else {
					if (bMustBeUnique)
						throw CNMRException(NMR_ERROR_OPCRELATIONSHIPNOTUNIQUE);
				}

			}
			iIterator++;
		}

		return pResultRelationship;
	}

	void COpcPackageReader::releaseZIP()
	{
		if (m_ZIParchive != nullptr)
			zip_close(m_ZIParchive);

		if (m_ZIPsource != nullptr)
			zip_source_close(m_ZIPsource);

		zip_error_fini(&m_ZIPError);
		m_Buffer.resize(0);

		m_ZIPsource = nullptr;
		m_ZIParchive = nullptr;
	}

	PImportStream COpcPackageReader::openZIPEntry(_In_ std::wstring sName)
	{
		auto iIterator = m_ZIPEntries.find(sName);
		if (iIterator == m_ZIPEntries.end()) {
			return nullptr;
		}

		return openZIPEntryIndexed(iIterator->second);


	}

	PImportStream COpcPackageReader::openZIPEntryIndexed(_In_ nfUint64 nIndex)
	{

		zip_stat_t Stat;
		nfInt32 nResult = zip_stat_index(m_ZIParchive, nIndex, ZIP_FL_UNCHANGED, &Stat);
		if (nResult != 0)
			throw CNMRException(NMR_ERROR_COULDNOTSTATZIPENTRY);

		nfUint64 nSize = Stat.size;

		zip_file_t * pFile = zip_fopen_index(m_ZIParchive, nIndex, ZIP_FL_UNCHANGED);
		if (pFile == nullptr)
			throw CNMRException(NMR_ERROR_COULDNOTOPENZIPENTRY);

		return std::make_shared<CImportStream_ZIP>(pFile, nSize);

	}


	void COpcPackageReader::readContentTypes()
	{
		// throw CNMRException(NMR_ERROR_NOTIMPLEMENTED)
	}

	void COpcPackageReader::readRootRelationships()
	{
		PImportStream pRelStream = openZIPEntry(OPCPACKAGE_PATH_ROOTRELATIONSHIPS);

		POpcPackageRelationshipReader pReader = std::make_shared<COpcPackageRelationshipReader>(pRelStream);

		nfUint32 nCount = pReader->getCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			m_RootRelationships.push_back(pReader->getRelationShip(nIndex));
		}
	}

	POpcPackagePart COpcPackageReader::createPart(_In_ std::wstring sPath)
	{
		std::wstring sRealPath = fnRemoveLeadingPathDelimiter (sPath);
		auto iPartIterator = m_Parts.find(sRealPath);
		if (iPartIterator != m_Parts.end()) {
			return iPartIterator->second;
		}

		PImportStream pStream = openZIPEntry(sRealPath);
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_COULDNOTCREATEOPCPART);

		POpcPackagePart pPart = std::make_shared<COpcPackagePart>(sRealPath, pStream);
		m_Parts.insert(std::make_pair(sRealPath, pPart));

		std::wstring sRelationShipName = fnExtractFileName(sRealPath);
		std::wstring sRelationShipPath = sRealPath.substr(0, sRealPath.length() - sRelationShipName.length());
		sRelationShipPath += L"_rels/";
		sRelationShipPath += sRelationShipName;
		sRelationShipPath += L".rels";

		PImportStream pRelStream = openZIPEntry(sRelationShipPath);

		if (pRelStream.get() != nullptr) {
		    POpcPackageRelationshipReader pReader = std::make_shared<COpcPackageRelationshipReader>(pRelStream);

		    nfUint32 nCount = pReader->getCount();
		    nfUint32 nIndex;

		    for (nIndex = 0; nIndex < nCount; nIndex++) {
		    	POpcPackageRelationship pRelationShip = pReader->getRelationShip(nIndex);
		    	pPart->addRelationship(pRelationShip->getID(), pRelationShip->getType(), pRelationShip->getTargetPartURI());
		    }
		}


		return pPart;
	}


}
