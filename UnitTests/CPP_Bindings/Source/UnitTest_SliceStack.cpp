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

UnitTest_SliceStack.cpp: Defines Unittests for the SliceStack class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class SliceStack : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			sliceStack = model->AddSliceStack(.0);
			mesh = model->AddMeshObject();

		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFSliceStack sliceStack;
		PLib3MFMeshObject mesh;
	};


	TEST_F(SliceStack, HasSetClearSliceStack)
	{
		ASSERT_FALSE(mesh->HasSliceStack());

		mesh->AssignSliceStack(sliceStack.get());
		ASSERT_TRUE(mesh->HasSliceStack());

		auto copySliceStack = mesh->GetSliceStack();
		ASSERT_EQ(copySliceStack->GetResourceID(), sliceStack->GetResourceID());

		mesh->ClearSliceStack();
		ASSERT_FALSE(mesh->HasSliceStack());
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


	class SliceStackArrangement : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
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

		PLib3MFModel model;
		PLib3MFSliceStack stackA, stackB, stackC;
		PLib3MFMeshObject mesh;
	};


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

		stackA->AddSliceStackReference(stackC.get());
		ASSERT_EQ(stackA->GetSliceRefCount(), 2);
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

}

