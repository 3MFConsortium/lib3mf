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
	class Reader : public ::testing::Test {
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

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Reader::wrapper;

	TEST_F(Reader, 3MFReadFromFile)
	{
		Reader::reader3MF->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.3mf");
		CheckReaderWarnings(Reader::reader3MF, 0);
	}

	TEST_F(Reader, STLReadFromFile)
	{
		Reader::readerSTL->ReadFromFile(sTestFilesPath + "/Reader/" + "Pyramid.stl");
		CheckReaderWarnings(Reader::readerSTL, 0);
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

		ASSERT_EQ(pVolumeIterator->MoveNext(), true);

	}

}
