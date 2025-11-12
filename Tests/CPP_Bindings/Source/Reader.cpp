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

UnitTest_Reader.cpp: Defines Unittests for the Reader classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

#include <algorithm>

namespace Lib3MF
{
	class Reader : public Lib3MFTest {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader3MF = model->QueryReader("3mf");
			readerSTL = model->QueryReader("stl");
		}
		virtual void TearDown() {
			model.reset();
			reader3MF.reset();
			readerSTL.reset();
		}

		void ExpectDegenerateTriangleResult();
	
		PModel model;
		PReader reader3MF;
		PReader readerSTL;
	};

	void Reader::ExpectDegenerateTriangleResult()
	{
		auto meshObjects = model->GetMeshObjects();
		const Lib3MF_uint32 expectedObjectCount = 1;
		ASSERT_EQ(expectedObjectCount, meshObjects->Count());
		ASSERT_TRUE(meshObjects->MoveNext());

		auto meshObject = meshObjects->GetCurrentMeshObject();
		const Lib3MF_uint32 expectedTriangleCount = 1;
		EXPECT_EQ(expectedTriangleCount, meshObject->GetTriangleCount());
		EXPECT_TRUE(meshObject->HasDegenerateTriangles());
		EXPECT_EQ(expectedTriangleCount, meshObject->GetDegenerateTriangleCount());

		Lib3MF_uint32 triangleElementIndex = 0;
		auto degenerateTriangle = meshObject->GetDegenerateTriangle(0, triangleElementIndex);
		const Lib3MF_uint32 expectedElementIndex = 1;
		EXPECT_EQ(expectedElementIndex, triangleElementIndex);
		const Lib3MF_uint32 expectedFirstIndex = 0;
		const Lib3MF_uint32 expectedSecondIndex = 0;
		const Lib3MF_uint32 expectedThirdIndex = 2;
		EXPECT_EQ(expectedFirstIndex, degenerateTriangle.m_Indices[0]);
		EXPECT_EQ(expectedSecondIndex, degenerateTriangle.m_Indices[1]);
		EXPECT_EQ(expectedThirdIndex, degenerateTriangle.m_Indices[2]);
	}

	TEST_F(Reader, 3MFReadFromFile)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

    TEST_F(Reader, 3MFReadFromFileAndAddMeshObjects)
    {
        auto reader = model->QueryReader("3mf");
        reader->ReadFromFile(sTestFilesPath + "/Reader/" + "Box.3mf");
        ASSERT_NO_THROW(model->AddMeshObject());
        ASSERT_NO_THROW(model->AddMeshObject());
        ASSERT_NO_THROW(model->AddMeshObject());

        auto objectIterator = model->GetObjects();
		const Lib3MF_uint32 expectedObjectCountWithAdditions = 5;
		EXPECT_EQ(expectedObjectCountWithAdditions, objectIterator->Count());
    }

    TEST_F(Reader, 3MFReadFromFileAndAddComponents)
    {
        auto reader = model->QueryReader("3mf");
        reader->ReadFromFile(sTestFilesPath + "/Reader/" + "Globo.3mf");
        ASSERT_NO_THROW(model->AddComponentsObject());
        ASSERT_NO_THROW(model->AddComponentsObject());
        ASSERT_NO_THROW(model->AddComponentsObject());

        auto objectIterator = model->GetObjects();
		const Lib3MF_uint32 expectedComponentCountWithAdditions = 8;
		EXPECT_EQ(expectedComponentCountWithAdditions, objectIterator->Count());
    }

	TEST_F(Reader, STLReadFromFile)
	{
		Reader::readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		CheckReaderWarnings(Reader::readerSTL, 0);
		auto meshObjects = model->GetMeshObjects();
		ASSERT_TRUE(meshObjects->Count() > 0);
		while (meshObjects->MoveNext()) {
			auto meshObject = meshObjects->GetCurrentMeshObject();
			ASSERT_TRUE(meshObject->GetVertexCount() > 0);
			ASSERT_TRUE(meshObject->GetTriangleCount() > 0);
		}
	}

	TEST_F(Reader, STLReadWriteRead)
	{
		Reader::readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		CheckReaderWarnings(Reader::readerSTL, 0);
		auto stlWriter = model->QueryWriter("stl");
		std::vector<Lib3MF_uint8> stlBuffer;
		stlWriter->WriteToBuffer(stlBuffer);
		auto tmpModel = wrapper->CreateModel();
		auto tmpReader = tmpModel->QueryReader("stl");
		tmpReader->ReadFromBuffer(stlBuffer);
	}

	TEST_F(Reader, 3MFReadFromBuffer)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		Reader::reader3MF->ReadFromBuffer(buffer);
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, STLReadFromBuffer)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		Reader::readerSTL->ReadFromBuffer(buffer);
		CheckReaderWarnings(Reader::readerSTL, 0);
	}

	TEST_F(Reader, 3MFReadFromCallback)
	{
		PositionedVector<Lib3MF_uint8> bufferCallback;
		bufferCallback.vec = ReadFileIntoBuffer(sTestFilesPath + "/Reader/" + "Pyramid.3mf");

		Reader::reader3MF->ReadFromCallback(
			PositionedVector<Lib3MF_uint8>::readCallback,
			bufferCallback.vec.size(),
			PositionedVector<Lib3MF_uint8>::seekCallback,
			reinterpret_cast<Lib3MF_pvoid>(&bufferCallback)
		);
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, DegenerateTriangleNonStrictMode)
	{
		reader3MF->SetStrictModeActive(false);
		reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "DegenerateTriangle.3mf");

		CheckReaderWarnings(reader3MF, 1);

		ExpectDegenerateTriangleResult();
	}

	TEST_F(Reader, DegenerateTriangleStrictModeAllowsFlag)
	{
		reader3MF->SetStrictModeActive(true);
	reader3MF->SetAllowDegenerateTriangles(true);
	EXPECT_TRUE(reader3MF->GetAllowDegenerateTriangles());
	reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "DegenerateTriangle.3mf");

	CheckReaderWarnings(reader3MF, 0);

	ExpectDegenerateTriangleResult();
}

	TEST_F(Reader, DegenerateTriangleStrictModeThrows)
	{
		reader3MF->SetStrictModeActive(true);
		ASSERT_SPECIFIC_THROW(reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "DegenerateTriangle.3mf"), ELib3MFException);
	}

	TEST_F(Reader, STLDegenerateTriangle)
	{
		readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "DegenerateTriangle.stl");
		CheckReaderWarnings(readerSTL, 0);

		ExpectDegenerateTriangleResult();
	}

	TEST_F(Reader, STLDegenerateTriangleASCII)
	{
		readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "DegenerateTriangle_ASCII.stl");
		CheckReaderWarnings(readerSTL, 0);

		ExpectDegenerateTriangleResult();
	}

	TEST_F(Reader, Production)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Production/" + "2ProductionBoxes.3mf");
		Reader::reader3MF->ReadFromBuffer(buffer);
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, ProductionExternalModel) {
		auto reader = model->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath + "/Production/" + "detachedmodel.3mf");
		const Lib3MF_uint32 expectedBuildItems = 27;
		const Lib3MF_uint32 expectedObjects = 28;
		ASSERT_EQ(expectedBuildItems, model->GetBuildItems()->Count());
		ASSERT_EQ(expectedObjects, model->GetObjects()->Count());
	}

	TEST_F(Reader, DuplicateStartPart)
	{
		// the /.rels-file of this 3MF contains two Relationship-entries with the StartPart relationship.
		// The 3MFReader should raise a warning when reading this file.
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "CS-1111_3dmodel-e1r1_other-e1r1.3mf");

		CheckReaderWarnings(Reader::reader3MF, 1);
	}

	TEST_F(Reader, BrokeComposite) {
		auto reader = model->QueryReader("3mf");
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Reader/" + "DOS-FUZ-LIB3MF.3mf");, ELib3MFException);
	}

	TEST_F(Reader, DuplicateTransformComponent) {
		auto reader = model->QueryReader("3mf");
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Reader/" + "GEN-C-ADA-COMPONENT-TRANSFORM-0.3mf"), ELib3MFException);
	}

	TEST_F(Reader, DuplicateTransformBuildItem) {
		auto reader = model->QueryReader("3mf");
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Reader/" + "GEN-M-ADA-ITEM-TRANSFORM-0.3mf"), ELib3MFException);
	}


	TEST_F(Reader, ReadVerticesCommaSeparatedValue) {
		// This file N_XXX_0422_01.3mf contains vertices with comma-separated values.
		// The 3MFReader should throw an error at NMR_StringUtils::fnStringToDouble when reading this file because 
		// comma-separated values are not allowed in 3MF files.
		auto reader = model->QueryReader("3mf");
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Reader/" + "N_XXX_0422_01.3mf"), ELib3MFException);
	}

	TEST_F(Reader, ReadVerticesWithLeadingPLUSSign) {
		// This file P_XXM_0519_01.3mf contains vertices with leading + sign e.g +1E+2.
		// The 3MFReader allows leading + sign at NMR_StringUtils::fnStringToDouble when reading this file.
		auto reader = model->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath + "/Reader/" + "P_XXM_0519_01.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, ReadVerticesValueWithLeadingTrialingSpaces) {
		// This file cam-51476-test.3mf contains vertices with leading whitespaces.
		// The 3MFReader allows leading/trialing whitespaces at NMR_StringUtils::fnStringToDouble when reading this file.
		auto reader = model->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath + "/Reader/" + "cam_51476_test.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);
	}
}
