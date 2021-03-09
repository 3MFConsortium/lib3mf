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

UnitTest_Writer.cpp: Defines Unittests for the Writer classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Writer : public ::testing::Test {
	protected:
		sPosition pVertices[8];
		sTriangle pTriangles[12];

		virtual void SetUp() {
			model = wrapper->CreateModel();
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(InFolder + "Pyramid.3mf");
			writer3MF = model->QueryWriter("3mf");
			writerSTL = model->QueryWriter("stl");

			float fSizeX = 100.0f;
			float fSizeY = 200.0f;
			float fSizeZ = 300.0f;

			// Manually create vertices
			pVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
			pVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
			pVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
			pVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
			pVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
			pVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
			pVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
			pVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

			// Manually create triangles
			pTriangles[0] = fnCreateTriangle(2, 1, 0);
			pTriangles[1] = fnCreateTriangle(0, 3, 2);
			pTriangles[2] = fnCreateTriangle(4, 5, 6);
			pTriangles[3] = fnCreateTriangle(6, 7, 4);
			pTriangles[4] = fnCreateTriangle(0, 1, 5);
			pTriangles[5] = fnCreateTriangle(5, 4, 0);
			pTriangles[6] = fnCreateTriangle(2, 3, 7);
			pTriangles[7] = fnCreateTriangle(7, 6, 2);
			pTriangles[8] = fnCreateTriangle(1, 2, 6);
			pTriangles[9] = fnCreateTriangle(6, 5, 1);
			pTriangles[10] = fnCreateTriangle(3, 0, 4);
			pTriangles[11] = fnCreateTriangle(4, 7, 3);
		}
		virtual void TearDown() {
			model.reset();
			writer3MF.reset();
			writerSTL.reset();
		}
	
		PModel model;
		PWriter writer3MF;
		PWriter writerSTL;

		static std::string InFolder;
		static std::string OutFolder;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Writer::wrapper;
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

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin()));
	}

	TEST_F(Writer, 3MFPrecision)
	{
		std::vector<sPosition> vctVertices;
		std::vector<sTriangle> vctTriangles;
		fnCreateBox(vctVertices, vctTriangles);
		auto mesh = model->AddMeshObject();
		mesh->SetGeometry(vctVertices, vctTriangles);

		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		Lib3MF_uint32 nPrecission = writer3MF->GetDecimalPrecision();
		writer3MF->SetDecimalPrecision(nPrecission+2);
		std::vector<Lib3MF_uint8> bufferLargr;
		Writer::writer3MF->WriteToBuffer(bufferLargr);

		ASSERT_TRUE(buffer.size() < bufferLargr.size());
	}

	TEST_F(Writer, STLCompare)
	{
		// This test is atleast functional
		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		Writer::writerSTL->WriteToFile(Writer::OutFolder + "Pyramid.stl");
		auto bufferFromFile = ReadFileIntoBuffer(Writer::OutFolder + "Pyramid.stl");

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), bufferFromFile.begin()));
	}

	TEST_F(Writer, 3MFWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writer3MF->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_pvoid>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writer3MF->WriteToBuffer(buffer);

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin()));
	}

	TEST_F(Writer, STLWriteToCallback)
	{
		PositionedVector<Lib3MF_uint8> callbackBuffer;
		Writer::writerSTL->WriteToCallback(PositionedVector<Lib3MF_uint8>::writeCallback,
			PositionedVector<Lib3MF_uint8>::seekCallback, reinterpret_cast<Lib3MF_pvoid>(&callbackBuffer));

		std::vector<Lib3MF_uint8> buffer;
		Writer::writerSTL->WriteToBuffer(buffer);

		ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), callbackBuffer.vec.begin()));
	}
}
