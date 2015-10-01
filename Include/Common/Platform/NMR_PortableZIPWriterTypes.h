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

NMR_PortableZIPWriterTypes.h defines a portable and fast writer of ZIP files

--*/

#ifndef __NMR_PORTABLEZIPWRITERTYPES
#define __NMR_PORTABLEZIPWRITERTYPES

#include "Common/NMR_Types.h"

#define ZIPFILEHEADERSIGNATURE 0x04034b50
#define ZIPFILECENTRALHEADERSIGNATURE 0x02014b50
#define ZIPFILEENDOFCENTRALDIRSIGNATURE 0x06054b50
#define ZIPFILEDATADESCRIPTORSIGNATURE 0x08074b5
#define ZIPFILEDESCRIPTOROFFSET 14
#define ZIPFILEVERSIONNEEDED 10
#define ZIPFILEEXTERNALFILEATTRIBUTES 0x80 // faArchive
#define ZIPFILEMAXENTRIES 2147483647 // 2^31 - 1

#define ZIPFILEFLAG_DATADESCRIPTORISUSED 0x08

#define ZIPFILEDATAZIP64EXTENDEDINFORMATIONEXTRAFIELD 0x0001

#define ZIPFILECOMPRESSION_UNCOMPRESSED 0
#define ZIPFILECOMPRESSION_DEFLATED 8
#define ZIPFILEMAXFILENAMELENGTH 32000

namespace NMR {

#pragma pack (1)

	typedef struct {
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
	} ZIPLOCALFILEHEADER;

	typedef struct {
		nfUint32 m_nCRC32;
		nfUint32 m_nCompressedSize;
		nfUint32 m_nUnCompressedSize;
	} ZIPLOCALFILEDESCRIPTOR;
		
	typedef struct {
		nfUint16 m_nHeaderID;
		nfUint16 m_nDataSize;
	} ZIPLOCALFILEADDITIONALHEADER;

	typedef struct {
		nfUint64 m_nUncompressedSize;
		nfUint64 m_nCompressedSize;
		nfUint64 m_nRelativeHeaderOffset;
		nfUint32 m_nDiskStartNumber;
	} ZIPLOCALFILEEXTENDEDINFORMATIONFIELD;

	typedef struct {
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
	} ZIPCENTRALDIRECTORYFILEHEADER;

	typedef struct {
		nfUint32 m_nSignature;
		nfUint16 m_nNumberOfDisk;
		nfUint16 m_nRelativeNumberOfDisk;
		nfUint16 m_nNumberOfEntriesOfDisk;
		nfUint16 m_nNumberOfEntriesOfDirectory;
		nfUint32 m_nSizeOfCentralDirectory;
		nfUint32 m_nOffsetOfCentralDirectory;
		nfUint16 m_nCommentLength;
	} ZIPENDOFCENTRALDIRHEADER;

#pragma pack()

}

#endif //__NMR_PORTABLEZIPWRITERTYPES
