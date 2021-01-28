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

NMR_OpcPackageReader.cpp defines an OPC Package reader in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackageReader.h" 
#include "Common/OPC/NMR_OpcPackageRelationshipReader.h" 
#include "Common/OPC/NMR_OpcPackageContentTypesReader.h" 
#include "Common/Platform/NMR_ImportStream_ZIP.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include "Model/Classes/NMR_ModelConstants.h"

#include <iostream>

namespace NMR {
	
	// custom callbck function for reading from a CImportStream on the fly
	zip_int64_t custom_zip_source_callback(void *userData, void *data, zip_uint64_t len, zip_source_cmd_t cmd) {
		if (userData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CImportStream* pImportStream = (CImportStream*)(userData);

		switch (cmd) {
			case ZIP_SOURCE_SUPPORTS:
				zip_int64_t bitmap;
				bitmap = zip_source_make_command_bitmap(ZIP_SOURCE_OPEN, ZIP_SOURCE_READ, ZIP_SOURCE_CLOSE,
					ZIP_SOURCE_STAT, ZIP_SOURCE_ERROR, ZIP_SOURCE_FREE, ZIP_SOURCE_SEEK, ZIP_SOURCE_TELL, ZIP_SOURCE_SUPPORTS, -1);
				return bitmap;

			case ZIP_SOURCE_SEEK:
				zip_source_args_seek argsSeek;
				argsSeek = * ((zip_source_args_seek *)data);
				if (argsSeek.whence == SEEK_SET)
					pImportStream->seekPosition(argsSeek.offset, true);
				else if (argsSeek.whence == SEEK_CUR) {
					pImportStream->seekPosition(pImportStream->getPosition() + argsSeek.offset, true);
				}
				else if (argsSeek.whence == SEEK_END) {
					if (argsSeek.offset > 0)
						throw CNMRException(NMR_ERROR_ZIPCALLBACK);
					pImportStream->seekFromEnd(-argsSeek.offset, true);
				}
				else
					throw CNMRException(NMR_ERROR_ZIPCALLBACK);
				return 0;

			case ZIP_SOURCE_OPEN:
				return 0;

			case ZIP_SOURCE_READ:
				return pImportStream->readBuffer((nfByte*)data, len, true);

			case ZIP_SOURCE_CLOSE:
				return 0;

			case ZIP_SOURCE_TELL:
				return pImportStream->getPosition();

			case ZIP_SOURCE_STAT:
				zip_stat_t* zipStat;
				zipStat  = (zip_stat_t*)data;
				zip_stat_init(zipStat);
				zipStat->size = pImportStream->retrieveSize();
				zipStat->valid |= ZIP_STAT_SIZE;
				return sizeof(zip_stat_t);

			case ZIP_SOURCE_FREE:
				return 0;

			default:
				throw CNMRException(NMR_ERROR_ZIPCALLBACK);
		}
		return -1;
	}

	COpcPackageReader::COpcPackageReader(_In_ PImportStream pImportStream, _In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor)
		: m_pWarnings(pWarnings), m_pProgressMonitor(pProgressMonitor)
	{
		if (!pImportStream)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!pProgressMonitor)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_ZIPError.str = nullptr;
		m_ZIPError.sys_err = 0;
		m_ZIPError.zip_err = 0;
		m_ZIParchive = nullptr;
		zip_source_t* pZIPsource = nullptr;

		try {
			// determine stream size
			nfUint64 nStreamSize = pImportStream->retrieveSize();
			pImportStream->seekPosition(0, true);

			if (nStreamSize == 0)
				throw CNMRException(NMR_ERROR_COULDNOTGETSTREAMPOSITION);

			// create ZIP objects
			zip_error_init(&m_ZIPError);

			bool bUseCallback = true;
			if (bUseCallback) {
				// read ZIP from callback: faster and requires less memory
				pZIPsource = zip_source_function_create(custom_zip_source_callback, pImportStream.get(), &m_ZIPError);
			}
			else {
				// read ZIP into memory
				m_Buffer.resize((size_t)nStreamSize);
				pImportStream->readBuffer(&m_Buffer[0], nStreamSize, true);
				pZIPsource = zip_source_buffer_create(&m_Buffer[0], (size_t)nStreamSize, 0, &m_ZIPError);
			}
			if (pZIPsource == nullptr)
				throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);

			m_ZIParchive = zip_open_from_source(pZIPsource, ZIP_RDONLY | ZIP_CHECKCONS, &m_ZIPError);
			if (m_ZIParchive == nullptr) {
				m_ZIParchive = zip_open_from_source(pZIPsource, ZIP_RDONLY, &m_ZIPError);
				if (m_ZIParchive == nullptr)
					throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);
				else
					m_pWarnings->addException(CNMRException(NMR_ERROR_ZIPCONTAINSINCONSISTENCIES), mrwInvalidMandatoryValue);
			}

			// Get ZIP Content
			nfInt64 nEntryCount = zip_get_num_entries(m_ZIParchive, ZIP_FL_UNCHANGED);
			if (nEntryCount < 0)
				throw CNMRException(NMR_ERROR_COULDNOTREADZIPFILE);

			// List and stat Entries
			nfUint64 nUnzippedFileSize = 0;
			for (nfInt64 nIndex = 0; nIndex < nEntryCount; nIndex++) {
				const char * pszName = zip_get_name(m_ZIParchive, (nfUint64) nIndex, ZIP_FL_ENC_GUESS);
				m_ZIPEntries.insert(std::make_pair(pszName, nIndex));

				zip_stat_t Stat;
				nfInt32 nResult = zip_stat_index(m_ZIParchive, nIndex, ZIP_FL_UNCHANGED, &Stat);
				if (nResult != 0)
					throw CNMRException(NMR_ERROR_COULDNOTSTATZIPENTRY);

				nUnzippedFileSize += Stat.size;
			}

			m_pProgressMonitor->SetMaxProgress(double(nUnzippedFileSize));
			m_pProgressMonitor->ReportProgressAndQueryCancelled(true);

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

	_Ret_maybenull_ COpcPackageRelationship * COpcPackageReader::findRootRelation(_In_ std::string sRelationType, _In_ nfBool bMustBeUnique)
	{
		COpcPackageRelationship * pResultRelationship = nullptr;
		auto iIterator = m_RootRelationships.begin();
		while (iIterator != m_RootRelationships.end()) {
			POpcPackageRelationship pRelationship = *iIterator;

			std::string sType = pRelationship->getType();
			if (sType == sRelationType) {
				if (pResultRelationship == nullptr) {
					pResultRelationship = pRelationship.get();
				}
				else {
					if (bMustBeUnique)
						m_pWarnings->addException(CNMRException(NMR_ERROR_OPCRELATIONSHIPNOTUNIQUE), eModelWarningLevel::mrwInvalidOptionalValue);
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

		zip_error_fini(&m_ZIPError);
		m_Buffer.resize(0);

		m_ZIParchive = nullptr;
	}

	PImportStream COpcPackageReader::openZIPEntry(_In_ std::string sName)
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
		PImportStream pContentStream = openZIPEntry(OPCPACKAGE_PATH_CONTENTTYPES);

		POpcPackageContentTypesReader pReader = std::make_shared<COpcPackageContentTypesReader>(pContentStream, m_pProgressMonitor);

		nfUint32 nCount = pReader->getCount();
		nfUint32 nIndex;

		std::string modelExtension = "";
		std::string modelPart = "";
		m_relationShipExtension = "";
		for (nIndex = 0; nIndex < nCount; nIndex++) {
			POpcPackageContentType pContentType = pReader->getContentType(nIndex);
			if (pContentType->m_contentType == PACKAGE_3D_RELS_CONTENT_TYPE) {
				m_relationShipExtension = pContentType->m_extension;
			}
			if (pContentType->m_contentType == PACKAGE_3D_MODEL_CONTENT_TYPE) {
				modelExtension = pContentType->m_extension;
			}
		}
		if (m_relationShipExtension.empty())
			throw CNMRException(NMR_ERROR_OPC_MISSING_EXTENSION_FOR_RELATIONSHIP);

		nCount = pReader->getOverrideCount();
		for (nIndex = 0; nIndex < nCount; nIndex++) {
			POpcPackageContentType_Override pOverrideContentType = pReader->getOverrideContentType(nIndex);
			if (pOverrideContentType->m_contentType == PACKAGE_3D_MODEL_CONTENT_TYPE) {
				modelPart = pOverrideContentType->m_partName;
			}
		}

		if (modelExtension.empty() && modelPart.empty())
			throw CNMRException(NMR_ERROR_OPC_MISSING_EXTENSION_FOR_MODEL);
	}

	void COpcPackageReader::readRootRelationships()
	{
		PImportStream pRelStream = openZIPEntry(OPCPACKAGE_PATH_ROOTRELATIONSHIPS);

		POpcPackageRelationshipReader pReader = std::make_shared<COpcPackageRelationshipReader>(pRelStream, m_pProgressMonitor);

		nfUint32 nCount = pReader->getCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			m_RootRelationships.push_back(pReader->getRelationShip(nIndex));
		}
	}

	nfUint64 COpcPackageReader::getPartSize(_In_ std::string sPath)
	{
		std::string sRealPath = fnRemoveLeadingPathDelimiter(sPath);
		auto iIterator = m_ZIPEntries.find(sRealPath);
		if (iIterator == m_ZIPEntries.end()) {
			return 0;
		}

		zip_stat_t Stat;
		nfInt32 nResult = zip_stat_index(m_ZIParchive, iIterator->second, ZIP_FL_UNCHANGED, &Stat);
		if (nResult != 0)
			throw CNMRException(NMR_ERROR_COULDNOTSTATZIPENTRY);

		return Stat.size;
	}

	POpcPackagePart COpcPackageReader::createPart(_In_ std::string sPath)
	{
		std::string sRealPath = fnRemoveLeadingPathDelimiter (sPath);
		auto iPartIterator = m_Parts.find(sRealPath);
		if (iPartIterator != m_Parts.end()) {
			return iPartIterator->second;
		}
		
		PImportStream pStream = openZIPEntry(sRealPath);
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_COULDNOTCREATEOPCPART);

		POpcPackagePart pPart = std::make_shared<COpcPackagePart>(sRealPath, pStream);
		m_Parts.insert(std::make_pair(sRealPath, pPart));

		std::string sRelationShipName = fnExtractFileName(sRealPath);
		std::string sRelationShipPath = sRealPath.substr(0, sRealPath.length() - sRelationShipName.length());
		sRelationShipPath += "_rels/";
		sRelationShipPath += sRelationShipName;
		sRelationShipPath += "."+m_relationShipExtension;

		PImportStream pRelStream = openZIPEntry(sRelationShipPath);

		if (pRelStream.get() != nullptr) {
			POpcPackageRelationshipReader pReader = std::make_shared<COpcPackageRelationshipReader>(pRelStream, m_pProgressMonitor);

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
