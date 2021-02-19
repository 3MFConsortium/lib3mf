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

NMR_PortableZIPWriterTypes.h defines a portable and fast writer of ZIP files

--*/

#ifndef __NMR_PORTABLEZIPWRITERTYPES
#define __NMR_PORTABLEZIPWRITERTYPES

#include "Common/NMR_Types.h"
#include "Common/NMR_Architecture_Utils.h"

#define ZIPFILEHEADERSIGNATURE 0x04034b50
#define ZIPFILECENTRALHEADERSIGNATURE 0x02014b50
#define ZIPFILEENDOFCENTRALDIRSIGNATURE 0x06054b50
#define ZIPFILEDATADESCRIPTORSIGNATURE 0x08074b50
#define ZIPFILEDESCRIPTOROFFSET 14
#define ZIPFILEVERSIONNEEDED 0x0A
#define ZIPFILEVERSIONNEEDEDZIP64 0x2D
#define ZIP64FILEENDOFCENTRALDIRRECORDSIGNATURE 0x06064b50
#define ZIP64FILEENDOFCENTRALDIRLOCATORSIGNATURE 0x07064b50

#define ZIPFILEEXTERNALFILEATTRIBUTES 0x80 // faArchive
#define ZIPFILEMAXENTRIES 2147483647 // 2^31 - 1

#define ZIPFILEDATAZIP64EXTENDEDINFORMATIONEXTRAFIELD 0x0001

#define ZIPFILECOMPRESSION_UNCOMPRESSED 0
#define ZIPFILECOMPRESSION_DEFLATED 8
#define ZIPFILEMAXFILENAMELENGTH 32000

#define ZIPFILEMAXIMUMSIZENON64 0xFFFFFFFF

namespace NMR {

#pragma pack (1)
	typedef struct ZIPLOCALFILEHEADER {
		nfUint32 m_nSignature;
		nfUint16 m_nVersion;
		nfUint16 m_nGeneralPurposeFlags;
		nfUint16 m_nCompressionMethod;
		nfUint16 m_nLastModTime;
		nfUint16 m_nLastModDate;
		nfUint32 m_nCRC32;
		nfUint32 m_nCompressedSize;
		nfUint32 m_nUnCompressedSize;
		nfUint16 m_nFileNameLength;
		nfUint16 m_nExtraFieldLength;
		void swapByteOrder() {
			m_nSignature = swapBytes(m_nSignature);
			m_nVersion = swapBytes(m_nVersion);
			m_nGeneralPurposeFlags = swapBytes(m_nGeneralPurposeFlags);
			m_nCompressionMethod = swapBytes(m_nCompressionMethod);
			m_nLastModTime = swapBytes(m_nLastModTime);
			m_nLastModDate = swapBytes(m_nLastModDate);
			m_nCRC32 = swapBytes(m_nCRC32);
			m_nCompressedSize = swapBytes(m_nCompressedSize);
			m_nUnCompressedSize = swapBytes(m_nUnCompressedSize);
			m_nFileNameLength = swapBytes(m_nFileNameLength);
			m_nExtraFieldLength = swapBytes(m_nExtraFieldLength);
		}
	} ZIPLOCALFILEHEADER;

	typedef struct ZIPLOCALFILEDESCRIPTOR {
		nfUint32 m_nCRC32;
		nfUint32 m_nCompressedSize;
		nfUint32 m_nUnCompressedSize;
		void swapByteOrder() {
			m_nCRC32 = swapBytes(m_nCRC32);
			m_nCompressedSize = swapBytes(m_nCompressedSize);
			m_nUnCompressedSize = swapBytes(m_nUnCompressedSize);
		}
	} ZIPLOCALFILEDESCRIPTOR;

	typedef struct ZIP64EXTRAINFORMATIONFIELD {
		nfUint16 m_nTag;
		nfUint16 m_nFieldSize;
		nfUint64 m_nUncompressedSize;
		nfUint64 m_nCompressedSize;
		void swapByteOrder() {
			m_nTag = swapBytes(m_nTag);
			m_nFieldSize = swapBytes(m_nFieldSize);
			m_nUncompressedSize = swapBytes(m_nUncompressedSize);
			m_nCompressedSize = swapBytes(m_nCompressedSize);
		}
	} ZIP64EXTRAINFORMATIONFIELD;

	typedef struct ZIPCENTRALDIRECTORYFILEHEADER {
		nfUint32 m_nSignature;
		nfUint16 m_nVersionMade;
		nfUint16 m_nVersionNeeded;
		nfUint16 m_nGeneralPurposeFlags;
		nfUint16 m_nCompressionMethod;
		nfUint16 m_nLastModTime;
		nfUint16 m_nLastModDate;
		nfUint32 m_nCRC32;
		nfUint32 m_nCompressedSize;
		nfUint32 m_nUnCompressedSize;
		nfUint16 m_nFileNameLength;
		nfUint16 m_nExtraFieldLength;
		nfUint16 m_nFileCommentLength;
		nfUint16 m_nDiskNumberStart;
		nfUint16 m_nInternalFileAttributes;
		nfUint32 m_nExternalFileAttributes;
		nfUint32 m_nRelativeOffsetOfLocalHeader;
		void swapByteOrder() {
			m_nSignature = swapBytes(m_nSignature);
			m_nVersionMade = swapBytes(m_nVersionMade);
			m_nVersionNeeded = swapBytes(m_nVersionNeeded);
			m_nGeneralPurposeFlags = swapBytes(m_nGeneralPurposeFlags);
			m_nCompressionMethod = swapBytes(m_nCompressionMethod);
			m_nLastModTime = swapBytes(m_nLastModTime);
			m_nLastModDate = swapBytes(m_nLastModDate);
			m_nCRC32 = swapBytes(m_nCRC32);
			m_nCompressedSize = swapBytes(m_nCompressedSize);
			m_nUnCompressedSize = swapBytes(m_nUnCompressedSize);
			m_nFileNameLength = swapBytes(m_nFileNameLength);
			m_nExtraFieldLength = swapBytes(m_nExtraFieldLength);
			m_nFileCommentLength = swapBytes(m_nFileCommentLength);
			m_nDiskNumberStart = swapBytes(m_nDiskNumberStart);
			m_nInternalFileAttributes = swapBytes(m_nInternalFileAttributes);
			m_nExternalFileAttributes = swapBytes(m_nExternalFileAttributes);
			m_nRelativeOffsetOfLocalHeader = swapBytes(m_nRelativeOffsetOfLocalHeader);
		}
	} ZIPCENTRALDIRECTORYFILEHEADER;

	typedef struct ZIPENDOFCENTRALDIRHEADER {
		nfUint32 m_nSignature;
		nfUint16 m_nNumberOfDisk;
		nfUint16 m_nRelativeNumberOfDisk;
		nfUint16 m_nNumberOfEntriesOfDisk;
		nfUint16 m_nNumberOfEntriesOfDirectory;
		nfUint32 m_nSizeOfCentralDirectory;
		nfUint32 m_nOffsetOfCentralDirectory;
		nfUint16 m_nCommentLength;
		void swapByteOrder() {
			m_nSignature = swapBytes(m_nSignature);
			m_nNumberOfDisk = swapBytes(m_nNumberOfDisk);
			m_nRelativeNumberOfDisk = swapBytes(m_nRelativeNumberOfDisk);
			m_nNumberOfEntriesOfDisk = swapBytes(m_nNumberOfEntriesOfDisk);
			m_nNumberOfEntriesOfDirectory = swapBytes(m_nNumberOfEntriesOfDirectory);
			m_nSizeOfCentralDirectory = swapBytes(m_nSizeOfCentralDirectory);
			m_nOffsetOfCentralDirectory = swapBytes(m_nOffsetOfCentralDirectory);
			m_nCommentLength = swapBytes(m_nCommentLength);
		}
	} ZIPENDOFCENTRALDIRHEADER;

	typedef struct ZIP64ENDOFCENTRALDIRHEADER {
		nfUint32 m_nSignature;
		nfUint64 m_nEndOfCentralDirHeaderRecord;
		nfUint16 m_nVersionMade;
		nfUint16 m_nVersionNeeded;
		nfUint32 m_nNumberOfDisk;
		nfUint32 m_nNumberOfDiskOfCentralDirectory;
		nfUint64 m_nTotalNumberOfEntriesOnThisDisk;
		nfUint64 m_nTotalNumberOfEntriesInCentralDirectory;
		nfUint64 m_nSizeOfCentralDirectory;
		nfUint64 m_nOffsetOfCentralDirectoryWithRespectToDisk;
		// zip64 extensible data sector, variable size; not used
		void swapByteOrder() {
			m_nSignature = swapBytes(m_nSignature);
			m_nEndOfCentralDirHeaderRecord = swapBytes(m_nEndOfCentralDirHeaderRecord);
			m_nVersionMade = swapBytes(m_nVersionMade);
			m_nVersionNeeded = swapBytes(m_nVersionNeeded);
			m_nNumberOfDisk = swapBytes(m_nNumberOfDisk);
			m_nNumberOfDiskOfCentralDirectory = swapBytes(m_nNumberOfDiskOfCentralDirectory);
			m_nTotalNumberOfEntriesOnThisDisk = swapBytes(m_nTotalNumberOfEntriesOnThisDisk);
			m_nTotalNumberOfEntriesInCentralDirectory = swapBytes(m_nTotalNumberOfEntriesInCentralDirectory);
			m_nSizeOfCentralDirectory = swapBytes(m_nSizeOfCentralDirectory);
			m_nOffsetOfCentralDirectoryWithRespectToDisk = swapBytes(m_nOffsetOfCentralDirectoryWithRespectToDisk);
		}
	} ZIP64ENDOFCENTRALDIRHEADER;

	typedef struct ZIP64ENDOFCENTRALDIRLOCATOR {
		nfUint32 m_nSignature;
		nfUint32 m_nNumberOfDiskWithStartOfZIP64EOCentralDir;
		nfUint64 m_nRelativeOffset;
		nfUint32 m_nTotalNumberOfDisk;
		void swapByteOrder() {
			m_nSignature = swapBytes(m_nSignature);
			m_nNumberOfDiskWithStartOfZIP64EOCentralDir = swapBytes(m_nNumberOfDiskWithStartOfZIP64EOCentralDir);
			m_nRelativeOffset = swapBytes(m_nRelativeOffset);
			m_nTotalNumberOfDisk = swapBytes(m_nTotalNumberOfDisk);
		}
	} ZIP64ENDOFCENTRALDIRLOCATOR;

#pragma pack()

}

#endif //__NMR_PORTABLEZIPWRITERTYPES
