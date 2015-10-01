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

NMR_PortableZIPWriter.cpp implements a portable and fast writer of ZIP files

--*/

#include "Common/Platform/NMR_PortableZIPWriter.h"
#include "Common/Platform/NMR_ExportStream_ZIP.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CPortableZIPWriter::CPortableZIPWriter(_In_ PExportStream pExportStream)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pExportStream = pExportStream;
		m_nCurrentEntryKey = 0;
		m_nNextEntryKey = 1;
		m_pCurrentEntry = nullptr;
		m_bIsFinished = false;

		if (m_pExportStream->getPosition() != 0)
			throw CNMRException(NMR_ERROR_EXPORTSTREAMNOTEMPTY);
	}

	CPortableZIPWriter::~CPortableZIPWriter()
	{
		if (!m_bIsFinished)
			writeDirectory();

	}

	PExportStream CPortableZIPWriter::createEntry(_In_ const std::wstring sName, _In_ nfTimeStamp nUnixTimeStamp)
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_ZIPALREADYFINISHED);
		// Finish old entry state
		closeEntry();

		// Initialize new entry state
		m_nCurrentEntryKey = m_nNextEntryKey;
		m_nNextEntryKey++;
		if (m_nNextEntryKey >= ZIPFILEMAXENTRIES)
			throw CNMRException(NMR_ERROR_ZIPENTRYOVERFLOW);

		// Convert Name to UTF8
		std::wstring sFilteredName = fnRemoveLeadingPathDelimiter(sName);
		std::string sUTF8Name = fnUTF16toUTF8(sFilteredName);
		if (sUTF8Name.length() > ZIPFILEMAXFILENAMELENGTH)
			throw CNMRException(NMR_ERRORINVALIDZIPNAME);
		nfUint32 nNameLength = (nfUint32)sUTF8Name.length();

		// Convert Timestamp to File Date
		nfUint32 nFileDate = 0;
		nfUint16 nLastModTime = nFileDate % 65536;
		nfUint16 nLastModDate = nFileDate / 65536;

		// Write local file header
		ZIPLOCALFILEHEADER LocalHeader;
		LocalHeader.m_nSignature = ZIPFILEHEADERSIGNATURE;
		LocalHeader.m_nVersion = ZIPFILEVERSIONNEEDED;
		LocalHeader.m_nGeneralPurposeFlags = 0;
		LocalHeader.m_nCompressionMethod = ZIPFILECOMPRESSION_DEFLATED;
		LocalHeader.m_nLastModTime = nLastModTime;
		LocalHeader.m_nLastModDate = nLastModDate;
		LocalHeader.m_nCRC32 = 0;
		LocalHeader.m_nCompressedSize = 0;
		LocalHeader.m_nUnCompressedSize = 0;
		LocalHeader.m_nFileNameLength = nNameLength;
		LocalHeader.m_nExtraFieldLength = 0;// sizeof(ZIPLOCALFILEADDITIONALHEADER) + sizeof(ZIPLOCALFILEEXTENDEDINFORMATIONFIELD);

		/*ZIPLOCALFILEADDITIONALHEADER AdditionalHeader;
		AdditionalHeader.m_nHeaderID = ZIPFILEDATAZIP64EXTENDEDINFORMATIONEXTRAFIELD;
		AdditionalHeader.m_nDataSize = sizeof(ZIPLOCALFILEEXTENDEDINFORMATIONFIELD);

		ZIPLOCALFILEEXTENDEDINFORMATIONFIELD ExtendedInformation;
		ExtendedInformation.m_nCompressedSize = 0;
		ExtendedInformation.m_nUncompressedSize = 0;
		ExtendedInformation.m_nDiskStartNumber = 0;
		ExtendedInformation.m_nRelativeHeaderOffset = 0; */

		// Write data to ZIP stream
		nfUint64 nFilePosition = m_pExportStream->getPosition();
		m_pExportStream->writeBuffer(&LocalHeader, sizeof(LocalHeader));
		m_pExportStream->writeBuffer(sUTF8Name.c_str(), nNameLength);
		//m_pExportStream->writeBuffer(&AdditionalHeader, sizeof(AdditionalHeader));

		nfUint64 nExtInfoPosition = m_pExportStream->getPosition();
		//m_pExportStream->writeBuffer(&ExtendedInformation, sizeof(ExtendedInformation));

		nfUint64 nDataPosition = m_pExportStream->getPosition();

		// create list entry
		m_pCurrentEntry = std::make_shared<CPortableZIPWriterEntry>(sUTF8Name, nLastModTime, nLastModDate, nFilePosition, nExtInfoPosition, nDataPosition);
		m_Entries.push_back(m_pCurrentEntry);

		// Return new ZIP Entry stream
		m_pCurrentStream = std::make_shared<CExportStream_ZIP>(this, m_nCurrentEntryKey);
		return m_pCurrentStream;
	}

	void CPortableZIPWriter::closeEntry()
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_ZIPALREADYFINISHED);

		if (m_pCurrentEntry.get() != nullptr) {
			// finish current stream writing
			CExportStream_ZIP * pZipStream = dynamic_cast<CExportStream_ZIP *>(m_pCurrentStream.get());
			if (pZipStream == nullptr)
				throw CNMRException(NMR_ERROR_NOEXPORTSTREAM);
			pZipStream->flushZIPStream();

			// Write CRC and Size
			ZIPLOCALFILEDESCRIPTOR FileDescriptor;
			FileDescriptor.m_nCRC32 = m_pCurrentEntry->getCRC32();
			FileDescriptor.m_nCompressedSize = (nfUint32) m_pCurrentEntry->getCompressedSize();
			FileDescriptor.m_nUnCompressedSize = (nfUint32) m_pCurrentEntry->getUncompressedSize();

			// Write File Descriptor to file
			m_pExportStream->seekPosition(m_pCurrentEntry->getFilePosition() + ZIPFILEDESCRIPTOROFFSET, true);
			m_pExportStream->writeBuffer(&FileDescriptor, sizeof(FileDescriptor));

			// Write Extended Information to file
			/*ZIPLOCALFILEEXTENDEDINFORMATIONFIELD ExtendedInformation;
			ExtendedInformation.m_nCompressedSize = m_pCurrentEntry->getCompressedSize();
			ExtendedInformation.m_nUncompressedSize = m_pCurrentEntry->getUncompressedSize();
			ExtendedInformation.m_nDiskStartNumber = 0;
			ExtendedInformation.m_nRelativeHeaderOffset = m_pCurrentEntry->getFilePosition();

			// Write Info Table
			m_pExportStream->seekPosition(m_pCurrentEntry->getExtInfoPosition(), true);
			//m_pExportStream->writeBuffer(&ExtendedInformation, sizeof(ExtendedInformation));
            */
			// Reset file pointer
			m_pExportStream->seekFromEnd(0, true);
		}

		m_pCurrentStream = nullptr;
		m_pCurrentEntry = nullptr;
		m_nCurrentEntryKey = 0;
	}

	void CPortableZIPWriter::calculateChecksum(_In_ nfUint32 nEntryKey, _In_ const void * pBuffer, _In_ nfUint32 cbUncompressedBytes)
	{
		if (m_pCurrentEntry.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRY);

		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (nEntryKey != m_nCurrentEntryKey)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRYKEY);

		if (cbUncompressedBytes > 0) {
			m_pCurrentEntry->calculateChecksum(pBuffer, cbUncompressedBytes);
			m_pCurrentEntry->increaseUncompressedSize(cbUncompressedBytes);
		}
	}


	void CPortableZIPWriter::writeDeflatedBuffer(_In_ nfUint32 nEntryKey, _In_ const void * pBuffer, _In_ nfUint32 cbCompressedBytes)
	{
		if (m_pCurrentEntry.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRY);

		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (nEntryKey != m_nCurrentEntryKey)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRYKEY);

		if (cbCompressedBytes > 0) {
			m_pExportStream->writeBuffer(pBuffer, cbCompressedBytes);
			m_pCurrentEntry->increaseCompressedSize(cbCompressedBytes);
		}
	}

	nfUint64 CPortableZIPWriter::getCurrentSize(_In_ nfUint32 nEntryKey)
	{
		if (m_pCurrentEntry.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRY);

		if (nEntryKey != m_nCurrentEntryKey)
			throw CNMRException(NMR_ERROR_INVALIDZIPENTRYKEY);
		return m_pCurrentEntry->getUncompressedSize();
	}


	void CPortableZIPWriter::writeDirectory()
	{
		closeEntry();

		nfUint64 nCentralDirStartPos = m_pExportStream->getPosition();
		auto iIterator = m_Entries.begin();
		while (iIterator != m_Entries.end()) {
			PPortableZIPWriterEntry pEntry = *iIterator;
			std::string sUTF8Name = pEntry->getUTF8Name();
			nfUint32 nNameLength = (nfUint32)sUTF8Name.length();

			ZIPCENTRALDIRECTORYFILEHEADER DirectoryHeader;
			DirectoryHeader.m_nSignature = ZIPFILECENTRALHEADERSIGNATURE;
			DirectoryHeader.m_nVersionMade = ZIPFILEVERSIONNEEDED;
			DirectoryHeader.m_nVersionNeeded = ZIPFILEVERSIONNEEDED;
			DirectoryHeader.m_nGeneralPurposeFlags = 0;
			DirectoryHeader.m_nCompressionMethod = ZIPFILECOMPRESSION_DEFLATED;
			DirectoryHeader.m_nLastModTime = pEntry->getLastModTime();
			DirectoryHeader.m_nLastModDate = pEntry->getLastModDate();
			DirectoryHeader.m_nCRC32 = pEntry->getCRC32();
			DirectoryHeader.m_nCompressedSize = (nfUint32) pEntry->getCompressedSize();
			DirectoryHeader.m_nUnCompressedSize = (nfUint32) pEntry->getUncompressedSize();
			DirectoryHeader.m_nFileNameLength = nNameLength;
			DirectoryHeader.m_nExtraFieldLength = 0;//sizeof(ZIPLOCALFILEADDITIONALHEADER) + sizeof(ZIPLOCALFILEEXTENDEDINFORMATIONFIELD);
			DirectoryHeader.m_nFileCommentLength = 0;
			DirectoryHeader.m_nDiskNumberStart = 0;
			DirectoryHeader.m_nInternalFileAttributes = 0;
			DirectoryHeader.m_nExternalFileAttributes = ZIPFILEEXTERNALFILEATTRIBUTES;
			DirectoryHeader.m_nRelativeOffsetOfLocalHeader = (nfUint32) pEntry->getFilePosition();

			/*ZIPLOCALFILEADDITIONALHEADER AdditionalHeader;
			AdditionalHeader.m_nHeaderID = ZIPFILEDATAZIP64EXTENDEDINFORMATIONEXTRAFIELD;
			AdditionalHeader.m_nDataSize = sizeof(ZIPLOCALFILEEXTENDEDINFORMATIONFIELD);

			ZIPLOCALFILEEXTENDEDINFORMATIONFIELD ExtendedInformation;
			ExtendedInformation.m_nCompressedSize = pEntry->getCompressedSize();
			ExtendedInformation.m_nUncompressedSize = pEntry->getUncompressedSize();
			ExtendedInformation.m_nDiskStartNumber = 0;
			ExtendedInformation.m_nRelativeHeaderOffset = 0; */

			m_pExportStream->writeBuffer(&DirectoryHeader, (nfUint64) sizeof(DirectoryHeader));
			m_pExportStream->writeBuffer(sUTF8Name.c_str(), nNameLength);
			//m_pExportStream->writeBuffer(&AdditionalHeader, sizeof(AdditionalHeader));
			//m_pExportStream->writeBuffer(&ExtendedInformation, sizeof(ExtendedInformation));

			iIterator++;
		}

		ZIPENDOFCENTRALDIRHEADER EndHeader;
		EndHeader.m_nSignature = ZIPFILEENDOFCENTRALDIRSIGNATURE;
		EndHeader.m_nNumberOfDisk = 0;
		EndHeader.m_nRelativeNumberOfDisk = 0;
		EndHeader.m_nNumberOfEntriesOfDisk = (nfUint16) m_Entries.size();
		EndHeader.m_nNumberOfEntriesOfDirectory = (nfUint16)m_Entries.size();
		EndHeader.m_nSizeOfCentralDirectory = (nfUint32)(m_pExportStream->getPosition() - nCentralDirStartPos);
		EndHeader.m_nOffsetOfCentralDirectory = (nfUint32)nCentralDirStartPos;
		EndHeader.m_nCommentLength = 0;

		m_pExportStream->writeBuffer(&EndHeader, (nfUint64) sizeof(EndHeader));

		m_bIsFinished = true;
	}


}
