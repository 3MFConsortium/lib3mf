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

UnitTest_Writer.cpp: Defines Unittests for the Writer classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Writer : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(InFolder + "Pyramid.3mf");
			writer3MF = model->QueryWriter("3mf");
			writerSTL = model->QueryWriter("stl");
		}
		virtual void TearDown() {
			model.reset();
			writer3MF.reset();
			writerSTL.reset();
		}
	
		static PLib3MFModel model;
		static PLib3MFWriter writer3MF;
		static PLib3MFWriter writerSTL;

		static std::string InFolder;
		static std::string OutFolder;
	};

	PLib3MFModel Writer::model;
	PLib3MFWriter Writer::writer3MF;
	PLib3MFWriter Writer::writerSTL;
	std::string Writer::InFolder(sTestFilesPath + "/Writer/");
	std::string Writer::OutFolder(sOutFilesPath + "/Writer/");
	

	TEST_F(Writer, 3MFWriteToFile)
	{
		 Writer::writer3MF->WriteToFile(Writer::OutFolder + "Pyramid.3mf");
	}

	TEST_F(Writer, STLWriteToFile)
	{
		Writer::writerSTL->WriteToFile(Writer::OutFolder + "Pyramid.stl");
	}

	TEST_F(Writer, 3MFWriteToBuffer)
	{
		// This is a bit silly, as it essentially performs two writes!
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);
	}

	TEST_F(Writer, STLWriteToBuffer)
	{
		// This is a bit silly, as it essentially performs two writes!
		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);
	}

	TEST_F(Writer, 3MFCompare)
	{
		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		Writer::writer3MF->WriteToFile(Writer::OutFolder + "Pyramid.3mf");
		auto bufferFromFile = ReadFileIntoBuffer(Writer::OutFolder + "Pyramid.3mf");

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin());
		ASSERT_TRUE(bAreEqual);
	}

	TEST_F(Writer, STLCompare)
	{
		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		Writer::writerSTL->WriteToFile(Writer::OutFolder + "Pyramid.stl");
		auto bufferFromFile = ReadFileIntoBuffer(Writer::OutFolder + "Pyramid.stl");

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin());
		ASSERT_TRUE(bAreEqual);
	}

	TEST_F(Writer, 3MFWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writer3MF->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_uint64>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin());
		ASSERT_TRUE(bAreEqual);
	}

	TEST_F(Writer, STLWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writerSTL->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_uint64>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin());
		ASSERT_TRUE(bAreEqual);
	}

}
