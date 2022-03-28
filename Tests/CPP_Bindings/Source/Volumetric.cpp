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

Vulometric.cpp: Defines Unittests for the Volumetric extension

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Volumetric : public ::testing::Test {
	protected:
		virtual void SetUp() {
			ASSERT_TRUE(CreateDir(OutFolder.c_str())) << L"Could not create folder.";
			model = wrapper->CreateModel();
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
	
		PModel model;
		PWriter writer3MF;
		PWriter writerSTL;

		static std::string InFolder;
		static std::string OutFolder;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;

		PImageStack SetupSheetsFromFile();
		PMeshObject GetMesh();
	};
	PWrapper Volumetric::wrapper;
	std::string Volumetric::InFolder(sTestFilesPath + "/Volumetric/");
	std::string Volumetric::OutFolder(sOutFilesPath + "/Volumetric/");
	

	PImageStack Volumetric::SetupSheetsFromFile()
	{
		PImageStack pImageStack = model->AddImageStack(821, 819, 11);
		for (int i = 0; i < 11; i++) {
			std::string sNumber = "_";
			int k = i + 1;
			if (k < 10) {
				sNumber = sNumber + "0";
			}
			sNumber = sNumber + std::to_string(k);

			pImageStack->CreateSheetFromFile(i, "/volume/layer" + sNumber + ".png", InFolder + "img" + sNumber + ".png");
		}
		return pImageStack;
	}

	PMeshObject Volumetric::GetMesh()
	{
		auto meshes = model->GetMeshObjects();
		meshes->MoveNext();
		return meshes->GetCurrentMeshObject();
	}

	TEST_F(Volumetric, VolumetricWriter)
	{
		auto pImage3D = SetupSheetsFromFile();

		//auto pVolumetricStack = model->AddVolumetricStack();
		//pVolumetricStack->AddDestinationChannel("channel", 0.0);
		//auto pLayer = pVolumetricStack->AddLayer(wrapper->GetIdentityTransform(), Lib3MF::eBlendMethod::Mix);
		//auto pChannelSelector = pLayer->AddChannelSelector(pImage3D.get(), "R", "channel");

		//auto theMesh = GetMesh();
		//auto volumeData = theMesh->VolumeData();

		//ASSERT_TRUE(volumeData->GetBoundary() == nullptr);
		//auto levelset = volumeData->CreateNewLevelset(pVolumetricStack.get());

		//Lib3MF::sTransform sTransform = wrapper->GetIdentityTransform();
		//Lib3MF::sTransform sObtainedTransform = levelset->SetTransform();
		//ASSERT_FLOAT_EQ(sObtainedTransform.m_Fields[1][1], 1.0f);
		//sTransform.m_Fields[1][1] = 2.0;
		//levelset->SetTransform(sTransform);
		//sObtainedTransform = levelset->SetTransform();
		//ASSERT_FLOAT_EQ(sObtainedTransform.m_Fields[1][1], sTransform.m_Fields[1][1]);

		//double dVal = 0.4;
		//levelset->SetSolidThreshold(dVal);
		//ASSERT_EQ(dVal, levelset->GetSolidThreshold());

		//std::string sChannelName("DoesNtWork");
		//ASSERT_SPECIFIC_THROW(levelset->SetChannel(sChannelName), ELib3MFException);
		//double dBackground = 0.0;
		//pVolumetricStack->GetDestinationChannel(0, sChannelName, dBackground);
		//levelset->SetChannel(sChannelName);
		//ASSERT_TRUE(levelset->GetChannel() == sChannelName);

		//Volumetric::writer3MF->WriteToFile(Volumetric::OutFolder + "ColoredVolume.3mf");
	}

	TEST_F(Volumetric, VolumetricProperties)
	{
		auto pImage3D = SetupSheetsFromFile();

		auto fieldFromImage3D = model->AddScalarFieldFromImage3D(pImage3D.get());
		fieldFromImage3D->SetChannel(Lib3MF::eChannelName::Red);
		fieldFromImage3D->SetFilter(Lib3MF::eTextureFilter::Nearest);
		fieldFromImage3D->SetOffset(1.2);
		fieldFromImage3D->SetScale(.5);
		fieldFromImage3D->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap, Lib3MF::eTextureTileStyle::Mirror, Lib3MF::eTextureTileStyle::Clamp);

		auto constantField = model->AddScalarFieldConstant();
		ASSERT_EQ(constantField->GetValue(), 0.);
		double dValue = -0.2;
		constantField->SetValue(dValue);
		ASSERT_EQ(constantField->GetValue(), dValue);

		auto theMesh = GetMesh();
		auto volumeData = theMesh->VolumeData();
		std::string propertyName = "MyProperty";
		auto theProperty = volumeData->AddPropertyFromScalarField(propertyName, fieldFromImage3D.get());
		ASSERT_EQ(volumeData->GetPropertyCount(), 1);
		ASSERT_TRUE(theProperty->GetName() == propertyName);
		ASSERT_TRUE(theProperty->IsRequired());
		theProperty->SetIsRequired(false);
		ASSERT_FALSE(theProperty->IsRequired());

		auto theConstantProperty = volumeData->AddPropertyFromScalarField("MyConstantProperty", constantField.get());
		ASSERT_EQ(volumeData->GetPropertyCount(), 2);

		writer3MF->WriteToFile(Volumetric::OutFolder + "MyProperties.3mf");

		{
			PModel ioModel = wrapper->CreateModel();
			PReader ioReader = ioModel->QueryReader("3mf");
			ioReader->ReadFromFile(Volumetric::OutFolder + "MyProperties.3mf");

			PWriter ioWriter = ioModel->QueryWriter("3mf");
			ioWriter->WriteToFile(Volumetric::OutFolder + "MyPropertiesReOut.3mf");
		}
	}

	TEST_F(Volumetric, VolumetricComposition)
	{
		auto pImage3D = SetupSheetsFromFile();

		auto fieldFromImage3D = model->AddScalarFieldFromImage3D(pImage3D.get());

		auto constantField = model->AddScalarFieldConstant();
		ASSERT_EQ(constantField->GetValue(), 0.);
		double dValue = -0.2;
		constantField->SetValue(dValue);
		ASSERT_EQ(constantField->GetValue(), dValue);

		auto composedScalarField = model->AddScalarFieldComposed();
		composedScalarField->ScalarFieldReference1()->SetFieldResourceID(fieldFromImage3D->GetUniqueResourceID());
		composedScalarField->ScalarFieldReference2()->SetFieldResourceID(constantField->GetUniqueResourceID());
		composedScalarField->SetMethod(Lib3MF::eCompositionMethod::WeightedSum);
		composedScalarField->SetFactor1(1.0);
		composedScalarField->SetFactor2(-1.0);

		auto theMesh = GetMesh();
		auto volumeData = theMesh->VolumeData();
		auto theBoundary = volumeData->CreateNewBoundary(composedScalarField.get());
		writer3MF->WriteToFile(Volumetric::OutFolder + "MyComposition.3mf");

		{
			PModel ioModel = wrapper->CreateModel();
			PReader ioReader = ioModel->QueryReader("3mf");
			ioReader->ReadFromFile(Volumetric::OutFolder + "MyComposition.3mf");

			auto scalarFieldIterator = ioModel->GetScalarFields();
			ASSERT_EQ(scalarFieldIterator->Count(), 3);

			ASSERT_TRUE(scalarFieldIterator->MoveNext());
			auto scalarField = scalarFieldIterator->GetCurrentScalarField();
			auto fieldFromImage3DReIn = ioModel->GetScalarFieldFromImage3DByID(scalarField->GetUniqueResourceID());
			CompareScalarFieldFromImage3D(ioModel, fieldFromImage3DReIn, model, fieldFromImage3D);

			ASSERT_TRUE(scalarFieldIterator->MoveNext());
			scalarField = scalarFieldIterator->GetCurrentScalarField();
			auto constantFieldReIn = ioModel->GetScalarFieldConstantByID(scalarField->GetUniqueResourceID());
			ASSERT_EQ(constantFieldReIn->GetValue(), constantField->GetValue());
			ASSERT_EQ(constantFieldReIn->GetName(), constantField->GetName());

			ASSERT_TRUE(scalarFieldIterator->MoveNext());
			scalarField = scalarFieldIterator->GetCurrentScalarField();
			auto composedScalarFieldReIn = ioModel->GetScalarFieldComposedByID(scalarField->GetUniqueResourceID());
			CompareScalarFieldComposed(ioModel, composedScalarFieldReIn, model, composedScalarField);
			ASSERT_FALSE(scalarFieldIterator->MoveNext());

			auto ioMeshObjects = ioModel->GetMeshObjects();
			ASSERT_EQ(ioMeshObjects->Count(), 1);
			ASSERT_TRUE(ioMeshObjects->MoveNext());
			auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
			auto ioVolumeData = ioMesh->VolumeData();
			CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

			PWriter ioWriter = ioModel->QueryWriter("3mf");
			ioWriter->WriteToFile(Volumetric::OutFolder + "MyCompositionReOut.3mf");
		}
	}

	TEST_F(Volumetric, VolumetricBoundary)
	{
		auto pImage3D = SetupSheetsFromFile();

		auto fieldFromImage3D = model->AddScalarFieldFromImage3D(pImage3D.get());
		fieldFromImage3D->SetChannel(Lib3MF::eChannelName::Red);

		auto theMesh = GetMesh();
		auto volumeData = theMesh->VolumeData();
		auto theBoundary = volumeData->CreateNewBoundary(fieldFromImage3D.get());
		theBoundary->SetSolidThreshold(.234);

		writer3MF->WriteToFile(Volumetric::OutFolder + "Boundary.3mf");
		{
			PModel ioModel = wrapper->CreateModel();
			PReader ioReader = ioModel->QueryReader("3mf");
			ioReader->ReadFromFile(Volumetric::OutFolder + "Boundary.3mf");

			auto ioMeshObjects = ioModel->GetMeshObjects();
			ASSERT_EQ(ioMeshObjects->Count(), 1);
			ASSERT_TRUE(ioMeshObjects->MoveNext());
			auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
			auto ioVolumeData = ioMesh->VolumeData();

			Lib3MF_uint32 nUniqueID = ioVolumeData->GetBoundary()->GetFieldResourceID();
			auto pScalarField = ioModel->GetScalarFieldByID(nUniqueID);
			CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

			PWriter ioWriter = ioModel->QueryWriter("3mf");
			ioWriter->WriteToFile(Volumetric::OutFolder + "BoundaryReOut.3mf");
		}
	}

	TEST_F(Volumetric, VolumetricColor)
	{
		//auto pImage3D = SetupSheetsFromFile();

		//auto vectorFieldFromImage3D = model->AddVector3DFieldFromImage3D(pImage3D.get());

		//auto theMesh = GetMesh();
		//auto volumeData = theMesh->VolumeData();
		//auto theBoundary = volumeData->CreateNewColor(vectorFieldFromImage3D.get());

		//writer3MF->WriteToFile(Volumetric::OutFolder + "Color.3mf");
		//{
		//	PModel ioModel = wrapper->CreateModel();
		//	PReader ioReader = ioModel->QueryReader("3mf");
		//	ioReader->ReadFromFile(Volumetric::OutFolder + "Color.3mf");

		//	PWriter ioWriter = ioModel->QueryWriter("3mf");
		//	ioWriter->WriteToFile(Volumetric::OutFolder + "ColorReOut.3mf");
		//}
	}

}
