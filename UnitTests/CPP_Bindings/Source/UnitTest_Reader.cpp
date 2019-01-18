/*++

Copyright (C) 2018 3MF Consortium

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

UnitTest_Reader.cpp: Defines Unittests for the Reader classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Reader : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			reader3MF = model->QueryReader("3mf");
			readerSTL = model->QueryReader("stl");
		}
		virtual void TearDown() {
			model.reset();
			reader3MF.reset();
			readerSTL.reset();
		}
	
		static PLib3MFModel model;
		static PLib3MFReader reader3MF;
		static PLib3MFReader readerSTL;
	};

	PLib3MFModel Reader::model;
	PLib3MFReader Reader::reader3MF;
	PLib3MFReader Reader::readerSTL;

	void CheckWarnings(PLib3MFReader reader)
	{
		EXPECT_EQ(reader->GetWarningCount(), 0);
		for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++)
		{
			Lib3MF_uint32 nErrorCode;
			std::string sWarning = reader->GetWarning(iWarning, nErrorCode);
			EXPECT_TRUE(true) << iWarning << ": " << nErrorCode << ", " << sWarning;
		}
	}

	TEST_F(Reader, 3MFReadFromFile)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		CheckWarnings(Reader::reader3MF);
	}

	TEST_F(Reader, STLReadFromFile)
	{
		Reader::readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		CheckWarnings(Reader::readerSTL);
	}

	TEST_F(Reader, 3MFReadFromBuffer)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		Reader::reader3MF->ReadFromBuffer(buffer);
		CheckWarnings(Reader::reader3MF);
	}

	TEST_F(Reader, STLReadFromBuffer)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		Reader::readerSTL->ReadFromBuffer(buffer);
		CheckWarnings(Reader::readerSTL);
	}

	TEST_F(Reader, 3MFReadFromCallback)
	{
		PositionedVector<Lib3MF_uint8> bufferCallback;
		bufferCallback.vec = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.3mf");

		Reader::reader3MF->ReadFromCallback(
			PositionedVector<Lib3MF_uint8>::readCallback,
			bufferCallback.vec.size(),
			PositionedVector<Lib3MF_uint8>::seekCallback,
			reinterpret_cast<Lib3MF_uint64>(&bufferCallback)
		);
		CheckWarnings(Reader::reader3MF);
	}

}
