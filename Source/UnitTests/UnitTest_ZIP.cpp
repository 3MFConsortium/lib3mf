/*++

Copyright (C) 2017 Autodesk Inc.
Copyright (C) 2015 Microsoft Corporation (Original Author)
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

UnitTest_MeshFormat_STL.cpp: Defines Unittests for the STL mesh format

--*/

#include "CppUnitTest.h"
#include "Common\NMR_Exception.h"
#ifdef NMR_COM_NATIVE
#include "Common\Platform\NMR_ImportStream_COM.h"
#include "Common\Platform\NMR_ExportStream_COM.h"
#else
#include "Common\Platform\NMR_ImportStream_GCC_Native.h"
#include "Common\Platform\NMR_ExportStream_GCC_Native.h"
#endif

#include "Common\Platform\NMR_ExportStream_ZIP.h"
#include "Common/Platform/NMR_Time.h"

#include "Common\Platform\NMR_PortableZIPWriter.h"
#include <algorithm>

#include <random>
#include "Common/Platform/NMR_Time.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NMR
{

	TEST_CLASS(ZIP_Test)
	{
	private:
		const static std::wstring sInPath;
		const static std::wstring sOutPath;
		const static std::string sText;
		nfUint64 writeEntryLarger(PPortableZIPWriter pZipWriter, std::string sName, nfUint64 nSize) {
			PExportStream pStream = pZipWriter->createEntry(sName, fnGetUnixTime());
			nfUint64 nUncompressedSize = 0;
			while (nUncompressedSize <= nSize) {
				for (int k = 0; k < 1024; k++) {
					pStream->writeBuffer(sText.data(), sText.length());
					nUncompressedSize += sText.length();
				}
			}
			return nUncompressedSize;
		}
	public:

		TEST_METHOD(ZIPExport_EntrySmaller4GB)
		{
			PExportStream pExportStreamManual(new CExportStream_GCC_Native((sOutPath + L"Manual64.zip").c_str()));
			PPortableZIPWriter pZipWriterManual = std::make_shared<CPortableZIPWriter>(pExportStreamManual, true);
			{
				PExportStream pStreamManual = pZipWriterManual->createEntry("Test/asd.txt", fnGetUnixTime());
				char sXYZ[] = { 'X','Y','Z', 13,10 };
				pStreamManual->writeBuffer(sXYZ, 5);
			}
			{
				PExportStream pStreamManual = pZipWriterManual->createEntry("fgh.txt", fnGetUnixTime());
				char sABC[] = { 'A','B','C', 13, 10 };
				pStreamManual->writeBuffer(sABC, 5);
			}
			/*
			PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"EntrySmaller4GB_NONzip64.zip").c_str()));
			PPortableZIPWriter pZipWriter = std::make_shared<CPortableZIPWriter>(pExportStream, false);
			PExportStream pStream = pZipWriter->createEntry(L"DeBelloGallico.txt", fnGetUnixTime());
			pStream->writeBuffer(sText.data(), sText.size());
			
			PExportStream pExportStream64(new CExportStream_GCC_Native((sOutPath + L"EntrySmaller4GB_zip64.zip").c_str()));
			PPortableZIPWriter pZipWriter64 = std::make_shared<CPortableZIPWriter>(pExportStream64, true);
			PExportStream pStream64 = pZipWriter64->createEntry(L"DeBelloGallico.txt", fnGetUnixTime());
			pStream64->writeBuffer(sText.data(), sText.size());
			*/
		}

		TEST_METHOD(ZIPExport_EntryLarger4GB)
		{
			PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"EntryLarger4GB_zip64.zip").c_str()));
			PPortableZIPWriter pZipWriter = std::make_shared<CPortableZIPWriter>(pExportStream, true);

			writeEntryLarger(pZipWriter, "DeBelloGallico.txt", 0x100010000);
		}

		TEST_METHOD(ZIPExport_AllUncompressedLarger4GB)
		{
			PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"AllUncompressedLarger4GB_zip64.zip").c_str()));
			PPortableZIPWriter pZipWriter = std::make_shared<CPortableZIPWriter>(pExportStream, true);
			writeEntryLarger(pZipWriter, "DeBelloGallico1.txt", 0x08F000000);
			writeEntryLarger(pZipWriter, "DeBelloGallico2.txt", 0x08F000000);
		}

//		// This test takes about WAY too long to be considered a unit test
		//TEST_METHOD(ZIPExport_CompresedLarger4GB)
		//{
		//	PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"ContentLarger4GB_ZipLarger4GB_zip64.zip").c_str()));
		//	PPortableZIPWriter pZipWriter = std::make_shared<CPortableZIPWriter>(pExportStream, true);
		//	
		//	for (int iEntry=0; iEntry < 3; iEntry++) {
		//		PExportStream pStream = pZipWriter->createEntry(std::wstring(L"random_")+std::to_wstring(iEntry)+L".data", fnGetUnixTime());
		//		{
		//			nfUint64 nUncompressedSize = 0;
		//			std::vector<nfUint64>vctData(1024);
		//			while (nUncompressedSize < nfUint64(iEntry + 1)*nfUint64(0x10FFFFFFF) ) {
		//				for (int k = 0; k < 1024; k++) {
		//					vctData[k] = rand();
		//				}
		//				pStream->writeBuffer(vctData.data(), 8*vctData.size());
		//				nUncompressedSize += 8*vctData.size();
		//			}
		//		}
		//	}
		//}
		
	};

	const std::wstring ZIP_Test::sInPath = L"../../TestFiles/VS_UnitTests/";
	const std::wstring ZIP_Test::sOutPath = L"../TestOutput/";
	const std::string ZIP_Test::sText = "Gallia est omnis divisa in partes tres, quarum unam incolunt Belgae, aliam Aquitani, tertiam, qui ipsorum lingua Celtae, nostra Galli appellantur.\nHi omnes lingua, institutis, legibus inter se differunt.\nGallos ab Aquitanis Garunna flumen, a Belgis Matrona et Sequana dividit.\nHorum omnium fortissimi sunt Belgae, propterea quod a cultu atque humanitate provinciae longissime absunt, minimeque ad eos mercatores saepe commeant atque ea quae ad effeminandos animos pertinent important, proximique sunt Germanis, qui trans Rhenum incolunt, quibuscum continenter bellum gerunt.\nQua de causa Helvetii quoque reliquos Gallos virtute praecedunt, quod fere cotidianis proeliis cum Germanis contendunt, cum aut suis finibus eos prohibent aut ipsi in eorum finibus bellum gerunt.\nEorum una pars, quam Gallos obtinere dictum est, initium capit a flumine Rhodano, continetur Garunna flumine, Oceano, finibus Belgarum, attingit etiam ab Sequanis et Helvetiis flumen Rhenum, vergit ad septentriones.\nBelgae ab extremis Galliae finibus oriuntur, pertinent ad inferiorem partem fluminis Rheni, spectant in septentrionem et orientem solem.\nAquitania a Garunna flumine ad Pyrenaeos montes et eam partem Oceani quae est ad Hispaniam pertinet; spectat inter occasum solis et septentriones.\n\n";
}
