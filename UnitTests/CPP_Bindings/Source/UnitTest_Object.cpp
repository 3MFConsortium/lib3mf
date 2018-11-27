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

UnitTest_Object.cpp: Defines Unittests for the object class and its descendants

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class ObjectT : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;
	};
	PLib3MFModel ObjectT::model;

	TEST_F(ObjectT, AddMesh)
	{
		auto mesh = model->AddMeshObject();
	}

	TEST_F(ObjectT, AddComponents)
	{
		auto component = model->AddComponentsObject();
	}

	TEST_F(ObjectT, EnsureNoSelfReference)
	{
		ASSERT_FALSE(true) << "TODO";
	}

	void AddHierarchy(PLib3MFModel model)
	{
		auto components = model->AddComponentsObject();
		auto componentsInner = model->AddComponentsObject();

		auto mesh = model->AddMeshObject();
		sLib3MFTransform t = getIdentityTransform();
		auto component1 = components->AddComponent(mesh.get(), t);
		auto componentInner = componentsInner->AddComponent(mesh.get(), t);
		t.m_Fields[3][2] = 10;
		auto component2 = components->AddComponent(componentsInner.get(), t);
	}

	TEST_F(ObjectT, AddComponentsHierarchy)
	{
		AddHierarchy(model);
	}

	void TestHierarchy(PLib3MFModel model)
	{
		
	}


	class ObjectAssembled : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			AddHierarchy(model);
		}
		virtual void TearDown() {
			model.reset();
		}

		static PLib3MFModel model;
	};
	PLib3MFModel ObjectAssembled::model;

	TEST_F(ObjectAssembled, TestComponentsHierarchy)
	{
		TestHierarchy(model);
	}

	TEST_F(ObjectAssembled, TestComponentsIterator)
	{
		auto componentsIterator = model->GetComponentsObjects();
		ASSERT_TRUE(componentsIterator->MoveNext());
		auto components1 = componentsIterator->GetCurrent();
		ASSERT_TRUE(componentsIterator->MoveNext());
		auto components2 = componentsIterator->GetCurrent();
		ASSERT_FALSE(componentsIterator->MoveNext());
	}
	
	TEST_F(ObjectAssembled, TestMeshIterator)
	{
		auto meshIterator = model->GetMeshObjects();
		ASSERT_TRUE(meshIterator->MoveNext());
		auto mesh1 = meshIterator->GetCurrent();
		ASSERT_FALSE(meshIterator->MoveNext());
	}

	TEST_F(ObjectAssembled, TestObjectIterator)
	{
		auto objectIterator = model->GetObjects();
		ASSERT_TRUE(objectIterator->MoveNext());
		ASSERT_TRUE(objectIterator->MoveNext());
		ASSERT_TRUE(objectIterator->MoveNext());
		ASSERT_FALSE(objectIterator->MoveNext());
	}


	class ObjectSingle : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			components = model->AddComponentsObject();
			mesh = model->AddMeshObject();
			component = components->AddComponent(mesh.get(), getIdentityTransform());
		}
		virtual void TearDown() {
			model.reset();
		}

		static PLib3MFModel model;
		static PLib3MFComponentsObject components;
		static PLib3MFComponent component;
		static PLib3MFMeshObject mesh;
	};
	PLib3MFModel ObjectSingle::model;
	PLib3MFComponentsObject ObjectSingle::components;
	PLib3MFComponent ObjectSingle::component;
	PLib3MFMeshObject ObjectSingle::mesh;

	TEST_F(ObjectSingle, Transform)
	{
		ASSERT_FALSE(true) << "TODO";
	}

	TEST_F(ObjectSingle, PartName)
	{
		ASSERT_FALSE(true) << "TODO";
	}

	TEST_F(ObjectSingle, PartNumber)
	{
		ASSERT_FALSE(true) << "TODO";
	}

	TEST_F(ObjectSingle, UUID)
	{
		ASSERT_FALSE(true) << "TODO";
	}
}


