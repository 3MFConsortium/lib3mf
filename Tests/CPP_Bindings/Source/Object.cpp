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

UnitTest_Object.cpp: Defines Unittests for the object class and its descendants

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class ObjectT : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
		}
		virtual void TearDown() {
			model.reset();
		}
	
		PModel model;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ObjectT::wrapper;

	TEST_F(ObjectT, AddMesh)
	{
		auto mesh = model->AddMeshObject();
	}

	TEST_F(ObjectT, AddComponents)
	{
		auto component = model->AddComponentsObject();
	}

	void AddHierarchy(PModel model)
	{
		auto components = model->AddComponentsObject();
		auto componentsInner = model->AddComponentsObject();

		auto mesh = model->AddMeshObject();
		sTransform t = getIdentityTransform();
		auto component1 = components->AddComponent(mesh.get(), t);
		auto componentInner = componentsInner->AddComponent(mesh.get(), t);
		t.m_Fields[3][2] = 10;
		auto component2 = components->AddComponent(componentsInner.get(), t);
	}

	TEST_F(ObjectT, AddComponentsHierarchy)
	{
		AddHierarchy(model);
	}

	TEST_F(ObjectT, EnsureNoSelfReference)
	{
		auto components = model->AddComponentsObject();
		try {
			components->AddComponent(components.get(), getIdentityTransform());
			ASSERT_FALSE(true) << "Self reference is forbidden.";
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(ObjectT, DISABLED_EnsureNoHigherSelfReference)
	{
		auto componentsOuter = model->AddComponentsObject();
		auto componentsInner = model->AddComponentsObject();
		componentsOuter->AddComponent(componentsInner.get(), getIdentityTransform());
		try {
			componentsInner->AddComponent(componentsOuter.get(), getIdentityTransform());
			ASSERT_FALSE(true) << "Self reference is forbidden.";
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
	}


	void TestHierarchy(PModel model)
	{
		{
			auto meshObjects = model->GetMeshObjects();
			ASSERT_TRUE(meshObjects->MoveNext());
			auto res = meshObjects->GetCurrent();
			auto mesh = model->GetMeshObjectByID(res->GetResourceID());
			ASSERT_TRUE(mesh->IsMeshObject());
			ASSERT_FALSE(meshObjects->MoveNext());
		}

		auto componentsObjects = model->GetComponentsObjects();
		ASSERT_TRUE(componentsObjects->MoveNext());
		{
			auto res = componentsObjects->GetCurrent();
			auto components1 = model->GetComponentsObjectByID(res->GetResourceID());

			ASSERT_TRUE(components1->IsComponentsObject());

			Lib3MF_uint32 count = components1->GetComponentCount();
			ASSERT_EQ(count, 2);
			{
				auto component0 = components1->GetComponent(0);
				auto component1 = components1->GetComponent(1);
				auto supposedToBeMesh = model->GetMeshObjectByID(component0->GetObjectResourceID());
				auto supposedToBeComponents = model->GetComponentsObjectByID(component1->GetObjectResourceID());
				ASSERT_TRUE(supposedToBeMesh->IsMeshObject());
				ASSERT_TRUE(supposedToBeComponents->IsComponentsObject());
			}
		}
		
		ASSERT_TRUE(componentsObjects->MoveNext());
		{
			auto res = componentsObjects->GetCurrent();
			auto components2 = model->GetComponentsObjectByID(res->GetResourceID());
			ASSERT_TRUE(components2->IsComponentsObject());
			Lib3MF_uint32 count = components2->GetComponentCount();
			ASSERT_EQ(count, 1);

			auto component0 = components2->GetComponent(0);
			auto supposedToBeMesh = model->GetMeshObjectByID(component0->GetObjectResourceID());
			ASSERT_TRUE(supposedToBeMesh->IsMeshObject());
		}

		ASSERT_FALSE(componentsObjects->MoveNext());
	}


	class ObjectAssembled : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			AddHierarchy(model);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ObjectAssembled::wrapper;

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
		virtual void SetUp() {
			model = wrapper->CreateModel();
			components = model->AddComponentsObject();
			mesh = model->AddMeshObject();
			component = components->AddComponent(mesh.get(), getIdentityTransform());
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PComponentsObject components;
		PComponent component;
		PMeshObject mesh;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ObjectSingle::wrapper;

	TEST_F(ObjectSingle, Transform)
	{
		ASSERT_FALSE(component->HasTransform());

		sTransform t = getIdentityTransform();
		t.m_Fields[2][1] = 2;
		component->SetTransform(t);

		ASSERT_TRUE(component->HasTransform());
		sTransform tOut = component->GetTransform();
		ASSERT_EQ(t.m_Fields[2][1], tOut.m_Fields[2][1]);
	}

	TEST_F(ObjectSingle, PartName)
	{
		std::string testName("SomeName");
		components->SetName(testName);
		ASSERT_TRUE(components->GetName().compare(testName) == 0);

		mesh->SetName(testName);
		ASSERT_TRUE(mesh->GetName().compare(testName) == 0);
	}

	TEST_F(ObjectSingle, PartNumber)
	{
		std::string testNumber("SomeName");
		components->SetPartNumber(testNumber);
		ASSERT_TRUE(components->GetPartNumber().compare(testNumber) == 0);

		mesh->SetPartNumber(testNumber);
		ASSERT_TRUE(mesh->GetPartNumber().compare(testNumber) == 0);
	}

	TEST_F(ObjectSingle, UUID_Component)
	{
		bool bHasUUID = true;
		// by default everything has an UUID
		std::string uuid = component->GetUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_FALSE(uuid.empty());

		try {
			component->SetUUID("NOUUID");
			ASSERT_TRUE(false);
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
		std::string inUUID("f1bcd2d0-6400-4785-9faf-5adde30adc08");
		component->SetUUID(inUUID);
		uuid = component->GetUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_TRUE(uuid.compare(inUUID) == 0);
	}

	TEST_F(ObjectSingle, UUID_Object)
	{
		bool bHasUUID = true;
		// by default everything has an UUID
		std::string uuid = components->GetUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_FALSE(uuid.empty());

		try {
			components->SetUUID("NOUUID");
			ASSERT_TRUE(false);
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
		std::string inUUID("2b0f37c2-812c-46e7-a6e5-91460c6dbc09");
		components->SetUUID(inUUID);
		uuid = components->GetUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_TRUE(uuid.compare(inUUID) == 0);
	}


	class ObjectThumbnail : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			components = model->AddComponentsObject();
			mesh = model->AddMeshObject();
			component = components->AddComponent(mesh.get(), getIdentityTransform());
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PComponentsObject components;
		PComponent component;
		PMeshObject mesh;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ObjectThumbnail::wrapper;


	TEST_F(ObjectThumbnail, ObjectThumbnail)
	{
		std::string sRelationShipPath("/Metadata/thumbnail.png");
		auto attachment = model->AddAttachment(sRelationShipPath, "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail");
		components->SetAttachmentAsThumbnail(attachment.get());

		ASSERT_TRUE(mesh->GetThumbnailAttachment() == nullptr);

		ASSERT_TRUE(components->GetThumbnailAttachment()->GetPath() == attachment->GetPath());

		attachment->SetRelationShipType("otherschema");
		ASSERT_SPECIFIC_THROW(mesh->SetAttachmentAsThumbnail(attachment.get()), ELib3MFException);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> vctBuffer;
		ASSERT_SPECIFIC_THROW(writer->WriteToBuffer(vctBuffer), ELib3MFException);
	}

	TEST_F(ObjectThumbnail, ReadObjectThumbnail)
	{
		auto readModel = wrapper->CreateModel();

		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath+"/Objects/HelixWithThumbnail.3mf");

		ASSERT_EQ(readModel->GetAttachmentCount(), 1);
		auto meshObjects = readModel->GetMeshObjects();
		ASSERT_EQ(meshObjects->MoveNext(), true);
		auto mesh = meshObjects->GetCurrentMeshObject();
		auto thumbnail = mesh->GetThumbnailAttachment();
		ASSERT_TRUE(thumbnail != nullptr);
		EXPECT_EQ(thumbnail->GetStreamSize(), 105627);
	}

	TEST_F(ObjectThumbnail, ReadInvalidObjectThumbnail)
	{
		auto readModel = wrapper->CreateModel();

		auto reader = readModel->QueryReader("3mf");
		reader->AddRelationToRead("otherrelationship");
		reader->ReadFromFile(sTestFilesPath + "/Objects/HelixWithThumbnail_IncorrectRelationship.3mf");
		CheckReaderWarnings(reader, 1);
	}

	TEST_F(ObjectThumbnail, WriteReadObjectThumbnail)
	{
		std::string sRelationShipPath("/Metadata/thumbnail.png");
		auto attachment = model->AddAttachment(sRelationShipPath, "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail");
		mesh->SetAttachmentAsThumbnail(attachment.get());

		attachment->ReadFromFile(sTestFilesPath + "/Attachments/thumbnail.png");

		std::vector<Lib3MF::sPosition> vctVertices;
		std::vector<Lib3MF::sTriangle> vctTriangles;
		fnCreateBox(vctVertices, vctTriangles);
		mesh->SetGeometry(vctVertices, vctTriangles);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> vctBuffer;
		writer->WriteToBuffer(vctBuffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(vctBuffer);

		auto readAttachment = readModel->FindAttachment(sRelationShipPath);
		ASSERT_EQ(readAttachment->GetStreamSize(), attachment->GetStreamSize());

		auto meshObjects = model->GetMeshObjects();
		ASSERT_EQ(meshObjects->MoveNext(), true);
		auto readMesh = meshObjects->GetCurrentMeshObject();

		auto meshAttachment = readMesh->GetThumbnailAttachment();
		ASSERT_TRUE(meshAttachment.get() != nullptr);
		ASSERT_EQ(readAttachment->GetStreamSize(), meshAttachment->GetStreamSize());
	}

}


