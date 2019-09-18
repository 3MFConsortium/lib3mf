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

UnitTest_SliceStack.cpp: Defines Unittests for the SliceStack class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

#include <cmath>

namespace Lib3MF
{
	class SliceStack : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();
			sliceStack = model->AddSliceStack(.0);
			mesh = model->AddMeshObject();

		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PSliceStack sliceStack;
		PMeshObject mesh;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStack::wrapper;


	TEST_F(SliceStack, HasSetClearSliceStack)
	{
		ASSERT_FALSE(mesh->HasSlices(false));

		mesh->AssignSliceStack(sliceStack.get());
		ASSERT_TRUE(mesh->HasSlices(false));

		auto copySliceStack = mesh->GetSliceStack();
		ASSERT_EQ(copySliceStack->GetResourceID(), sliceStack->GetResourceID());

		mesh->ClearSliceStack();
		ASSERT_FALSE(mesh->HasSlices(false));
	}

	TEST_F(SliceStack, SliceResolution)
	{
		ASSERT_FALSE(mesh->HasSlices(false));

		eSlicesMeshResolution res = mesh->GetSlicesMeshResolution();
		ASSERT_EQ(res, eSlicesMeshResolution::Fullres);

		mesh->SetSlicesMeshResolution(eSlicesMeshResolution::Lowres);
		ASSERT_EQ(mesh->GetSlicesMeshResolution(), eSlicesMeshResolution::Lowres);

		mesh->SetSlicesMeshResolution(eSlicesMeshResolution::Fullres);
		ASSERT_EQ(mesh->GetSlicesMeshResolution(), eSlicesMeshResolution::Fullres);
	}

	TEST_F(SliceStack, AddSlicesNegative)
	{
		ASSERT_EQ(sliceStack->GetSliceCount(), 0);

		ASSERT_SPECIFIC_THROW(sliceStack->AddSlice(-1.0), ELib3MFException);

		auto slice = sliceStack->AddSlice(1.0);
		ASSERT_EQ(sliceStack->GetSliceCount(), 1);

		ASSERT_SPECIFIC_THROW(sliceStack->AddSlice(0.5), ELib3MFException);
	}

	TEST_F(SliceStack, Properties)
	{
		auto s = model->AddSliceStack(1.0);
		ASSERT_DOUBLE_EQ(s->GetBottomZ(), 1.0);

		ASSERT_EQ(s->GetSliceCount(), 0);

		ASSERT_EQ(s->GetSliceRefCount(), 0);
	}

	TEST_F(SliceStack, WorkWithSlices)
	{
		auto stack = model->AddSliceStack(1.0);
		auto sliceA = stack->AddSlice(2.);
		ASSERT_EQ(stack->GetSliceCount(), 1);

		auto sliceA1 = stack->GetSlice(0);

		auto sliceB = stack->AddSlice(3.);
		ASSERT_EQ(stack->GetSliceCount(), 2);
		auto sliceAB1 = stack->GetSlice(0);
	}


	class SliceStackArrangement : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			stackA = model->AddSliceStack(.0);
			stackB = model->AddSliceStack(0);
			stackC = model->AddSliceStack(10.0);
			mesh = model->AddMeshObject();

			stackB->AddSlice(5.0);
			stackC->AddSlice(15.0);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PSliceStack stackA, stackB, stackC;
		PMeshObject mesh;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackArrangement::wrapper;


	TEST_F(SliceStackArrangement, ForbiddenSelfReference)
	{
		ASSERT_SPECIFIC_THROW(stackA->AddSliceStackReference(stackA.get()), ELib3MFException);
	}

	TEST_F(SliceStackArrangement, ForbiddenSlicesAndSliceRefs)
	{
		ASSERT_SPECIFIC_THROW(stackB->AddSliceStackReference(stackA.get()), ELib3MFException);

		stackA->AddSlice(1.0);
		ASSERT_SPECIFIC_THROW(stackA->AddSliceStackReference(stackB.get()), ELib3MFException);
	}


	TEST_F(SliceStackArrangement, WorkWithRefs)
	{
		ASSERT_EQ(stackA->GetSliceRefCount(), 0);

		stackA->AddSliceStackReference(stackB.get());
		ASSERT_EQ(stackA->GetSliceRefCount(), 1);

		auto stackB2 = stackA->GetSliceStackReference(0);
		ASSERT_EQ(stackB2->GetResourceID(), stackB->GetResourceID());

		stackA->AddSliceStackReference(stackC.get());
		ASSERT_EQ(stackA->GetSliceRefCount(), 2);

		auto stackC2 = stackA->GetSliceStackReference(1);
		ASSERT_EQ(stackC2->GetResourceID(), stackC->GetResourceID());
	}

	TEST_F(SliceStackArrangement, InvalidRefOrder)
	{
		stackA->AddSliceStackReference(stackC.get());
		ASSERT_SPECIFIC_THROW(stackA->AddSliceStackReference(stackB.get()), ELib3MFException);
	}


	TEST_F(SliceStackArrangement, InvalidSlicesInReferences)
	{
		auto s1 = model->AddSliceStack(0.0);
		auto s2 = model->AddSliceStack(10.0);

		s1->AddSliceStackReference(s2.get());
		ASSERT_SPECIFIC_THROW(s1->AddSlice(15.0), ELib3MFException);
	}

	TEST_F(SliceStackArrangement, DISABLED_MustFail_AddRefOfRef1)
	{
		auto stack1 = model->AddSliceStack(0.);
		auto stack2 = model->AddSliceStack(1.);
		auto stack3 = model->AddSliceStack(2.);
		stack1->AddSliceStackReference(stack2.get());

		ASSERT_SPECIFIC_THROW(stack2->AddSliceStackReference(stack3.get()), ELib3MFException);
	}

	TEST_F(SliceStackArrangement, MustFail_AddRefOfRef2)
	{
		auto stack1 = model->AddSliceStack(0.);
		auto stack2 = model->AddSliceStack(1.);
		auto stack3 = model->AddSliceStack(2.);

		stack2->AddSliceStackReference(stack3.get());

		ASSERT_SPECIFIC_THROW(stack1->AddSliceStackReference(stack2.get()), ELib3MFException);
	}

	TEST_F(SliceStackArrangement, MustFail_DoubleAssignementOfReference)
	{
		stackA->AddSliceStackReference(stackB.get());
		ASSERT_SPECIFIC_THROW(stackA->AddSliceStackReference(stackB.get()), ELib3MFException);
	}

	TEST_F(SliceStackArrangement, Collapse)
	{
		stackA->AddSliceStackReference(stackB.get());
		stackA->AddSliceStackReference(stackC.get());

		stackA->CollapseSliceReferences();

		ASSERT_EQ(stackA->GetSliceCount(), 2);
	}

	TEST_F(SliceStackArrangement, GetSliceStacks)
	{
		auto stacks = model->GetSliceStacks();
		int count = 0;
		while (stacks->MoveNext()) {
			auto stack = stacks->GetCurrent();
			count++;
		}
		ASSERT_EQ(count, 3);
	}


	class SliceStackWriting : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			stackWithSlices = model->AddSliceStack(1.);
			auto slice = stackWithSlices->AddSlice(2.);

			writer = model->QueryWriter("3mf");

			std::vector<sPosition2D> vVertices;
			sPosition2D pos;
			pos.m_Coordinates[0] = 0;
			pos.m_Coordinates[1] = 0;
			vVertices.push_back(pos);

			pos.m_Coordinates[0] = 0;
			pos.m_Coordinates[1] = 1;
			vVertices.push_back(pos);

			pos.m_Coordinates[0] = 1;
			pos.m_Coordinates[1] = 1;
			vVertices.push_back(pos);

			pos.m_Coordinates[0] = 1;
			pos.m_Coordinates[1] = 0;
			vVertices.push_back(pos);

			std::vector<Lib3MF_uint32> vOpenPolygon;
			vOpenPolygon.push_back(0);
			vOpenPolygon.push_back(1);
			vOpenPolygon.push_back(2);

			slice->SetVertices(vVertices);
			slice->AddPolygon(vOpenPolygon);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PSliceStack stackWithSlices;
		PWriter writer;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackWriting::wrapper;

	void CompareSliceStacks(PSliceStack A, PSliceStack B)
	{
		EXPECT_TRUE(std::fabs(A->GetBottomZ() - B->GetBottomZ()) < 1e-5);
		EXPECT_TRUE(A->GetOwnPath() == B->GetOwnPath());
		ASSERT_EQ(A->GetSliceCount(), B->GetSliceCount());
		for (int i = 0; i < A->GetSliceCount(); i++) {
			auto slice = A->GetSlice(i);
			auto readSlice = B->GetSlice(i);

			ASSERT_EQ(slice->GetPolygonCount(), readSlice->GetPolygonCount());
			ASSERT_EQ(slice->GetVertexCount(), readSlice->GetVertexCount());
		}

		ASSERT_EQ(A->GetSliceRefCount(), B->GetSliceRefCount());
		for (int i = 0; i < A->GetSliceRefCount(); i++) {
			auto sliceRef = A->GetSliceStackReference(i);
			auto readSliceRef = B->GetSliceStackReference(i);

			CompareSliceStacks(sliceRef, readSliceRef);
		}
	}

	void checkSliceModels(PModel A, PModel B)
	{
		auto stacks = A->GetSliceStacks();
		auto readStacks = B->GetSliceStacks();
		while (readStacks->MoveNext()) {
			ASSERT_TRUE(stacks->MoveNext());

			auto stack = A->GetSliceStackByID(stacks->GetCurrent()->GetResourceID());
			auto readStack = B->GetSliceStackByID(readStacks->GetCurrent()->GetResourceID());

			CompareSliceStacks(stack, readStack);
		}
	}

	TEST_F(SliceStackWriting, WriteSlices)
	{
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);

		checkSliceModels(model, readModel);
	}

	TEST_F(SliceStackWriting, WriteSliceReference)
	{
		auto stack2 = model->AddSliceStack(0);
		stack2->AddSliceStackReference(stackWithSlices.get());

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);

		checkSliceModels(model, readModel);
	}

	TEST_F(SliceStackWriting, WriteSliceReferenceOutOfPlace)
	{
		auto stack3 = model->AddSliceStack(0);
		auto stack2 = model->AddSliceStack(5);
		auto stack1 = model->AddSliceStack(0);
		stack1->SetOwnPath("/2D/A2dmodel.model");
		stack2->SetOwnPath("/2D/A2dmodel.model");

		stack3->AddSliceStackReference(stack1.get());
		stack3->AddSliceStackReference(stack2.get());

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		{
			auto reader = readModel->QueryReader("3mf");
			reader->ReadFromBuffer(buffer);
		}


		auto readModelAgain = wrapper->CreateModel();
		{
			// write write and read again, since a true "fixpoint" is only reached after one more iteration
			auto writerAgain = readModel->QueryWriter("3mf");
			writerAgain->WriteToBuffer(buffer);

			auto readerAgain = readModelAgain->QueryReader("3mf");
			readerAgain->ReadFromBuffer(buffer);
		}
		checkSliceModels(readModel, readModelAgain);
	}



	class SliceStackReading : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PReader reader;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackReading::wrapper;

	TEST_F(SliceStackReading, ReadSlices)
	{
		reader->ReadFromFile(sTestFilesPath + "/Slice/Expected_Slice_Hierarchy.3mf");

		auto sliceStack1 = model->GetSliceStackByID(1);
		auto sliceStack2 = model->GetSliceStackByID(2);

		auto sliceStack3 = model->GetSliceStackByID(3);
		ASSERT_EQ(sliceStack3->GetSliceCount(), 1);
		auto slice0 = sliceStack3->GetSlice(0);
		ASSERT_EQ(slice0->GetPolygonCount(), 1);
		ASSERT_EQ(slice0->GetVertexCount(), 4);

		auto sliceStack4 = model->GetSliceStackByID(4);
		ASSERT_EQ(sliceStack4->GetSliceRefCount(), 2);

		ASSERT_EQ(sliceStack4->GetSliceRefCount(), 2);
		auto sr2 = sliceStack4->GetSliceStackReference(0);
		auto sr1 = sliceStack4->GetSliceStackReference(1);

		CompareSliceStacks(sr2, sliceStack2);
		CompareSliceStacks(sr1, sliceStack1);
		ASSERT_TRUE(sr1->GetOwnPath() == "/2D/A2dmodel.model");
	}

	class SliceStackReadingMultiple : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PReader reader;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackReadingMultiple::wrapper;

	INSTANTIATE_TEST_SUITE_P(WorkingFiles, SliceStackReadingMultiple,
		::testing::Values(
			"MultiSliceStack_TwoFiles.3mf",
			"MultiSliceStack_OneFile.3mf",
			"MultiSliceStack_NoFile.3mf",
			"UniSliceStack_TwoFiles.3mf"
		));

	TEST_P(SliceStackReadingMultiple, WorkingFiles)
	{
		std::string fileName = GetParam();
		reader->ReadFromFile(sTestFilesPath + "/Slice/" + fileName);
	}


	class SliceStackReadingMultipleFailing : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PReader reader;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackReadingMultipleFailing::wrapper;

	INSTANTIATE_TEST_SUITE_P(FailingFiles, SliceStackReadingMultipleFailing,
		::testing::Values(
			"Slice_MustFail_TooRefsTooDeep.3mf"
		));

	TEST_P(SliceStackReadingMultipleFailing, FailingFiles)
	{
		std::string fileName = GetParam();
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Slice/" + fileName), ELib3MFException);
	}


	class SliceStackTransform : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PReader reader;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SliceStackTransform::wrapper;

	TEST_F(SliceStackTransform, ReadSlices)
	{
		reader->ReadFromFile(sTestFilesPath + "/Slice/PS_505_02_non_planar_transform.3mf");
		EXPECT_EQ(reader->GetWarningCount(), 1);

		auto writer = model->QueryWriter("3mf");

		std::vector<Lib3MF_uint8> vctBuffer;
		ASSERT_SPECIFIC_THROW(writer->WriteToBuffer(vctBuffer), ELib3MFException);
	}
}

