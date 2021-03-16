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
	
		PModel model;
		PReader reader3MF;
		PReader readerSTL;
	};

	TEST_F(Reader, 3MFReadFromFile)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);
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

	TEST_F(Reader, Production)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Production/" + "2ProductionBoxes.3mf");
		Reader::reader3MF->ReadFromBuffer(buffer);
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, ProductionExternalModel) {
		auto reader = model->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath + "/Production/" + "detachedmodel.3mf");
		ASSERT_EQ(27, model->GetBuildItems()->Count());
		ASSERT_EQ(28, model->GetObjects()->Count());
	}

	TEST_F(Reader, 3MFReadVolumetric)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "PyramidVolume.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);

		auto pIterator = model->GetImage3Ds();
		ASSERT_EQ (pIterator->MoveNext (), true);
		auto pImage3D = pIterator->GetCurrentImage3D();
		ASSERT_EQ(pImage3D->GetSizeX(), 10);
		ASSERT_EQ(pImage3D->GetSizeY(), 10);
		ASSERT_EQ(pImage3D->GetSheetCount(), 3);

		ASSERT_EQ(pIterator->MoveNext(), false);

		auto pVolumeIterator = model->GetVolumetricStacks();
		ASSERT_EQ(pVolumeIterator->MoveNext(), true);
		auto pVolumetricStack = pVolumeIterator->GetCurrentVolumetricStack();

		ASSERT_EQ(pVolumeIterator->MoveNext(), false);
	}

	TEST_F(Reader, 3MFReadVolumetric2)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Volumetric/" + "ColoredVolume.3mf");
		CheckReaderWarnings(Reader::reader3MF, 1); // missing Image3D Name

		auto pIterator = model->GetImage3Ds();
		ASSERT_EQ(pIterator->MoveNext(), true);
		auto pImage3D = pIterator->GetCurrentImage3D();
		ASSERT_EQ(pImage3D->GetSizeX(), 821);
		ASSERT_EQ(pImage3D->GetSizeY(), 819);
		ASSERT_EQ(pImage3D->GetSheetCount(), 11);

		ASSERT_EQ(pIterator->MoveNext(), false);

		auto pVolumeIterator = model->GetVolumetricStacks();
		ASSERT_EQ(pVolumeIterator->MoveNext(), true);
		auto pVolumetricStack = pVolumeIterator->GetCurrentVolumetricStack();

		ASSERT_EQ(pVolumetricStack->GetDestinationChannelCount(), 1);

		ASSERT_EQ(pVolumeIterator->MoveNext(), false);

		auto meshObjects = model->GetMeshObjects();
		ASSERT_EQ(meshObjects->MoveNext(), true);
		auto mesh = meshObjects->GetCurrentMeshObject();

		auto levelset = mesh->VolumeData()->GetLevelset();
		ASSERT_TRUE(levelset != nullptr);

		std::string sChannelName;
		double dValue;
		pVolumetricStack->GetDestinationChannel(0, sChannelName, dValue);
		ASSERT_TRUE(levelset->GetChannel() == sChannelName);

		ASSERT_DOUBLE_EQ(levelset->GetSolidThreshold(), 0.4);
		auto pLevelsetStack = levelset->GetVolumetricStack();
		ASSERT_EQ(pLevelsetStack->GetResourceID(), pVolumetricStack->GetResourceID());

		sLib3MFTransform sTransform = levelset->GetTransform();
		ASSERT_FLOAT_EQ(sTransform.m_Fields[1][1], 2.0f);
	}
}
