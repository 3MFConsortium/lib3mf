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
    namespace helper
    {
        bool directoryExists(std::string& path)
        {
            struct stat info;
            if(stat(path.c_str(), &info) != 0)
                return false;
            else if(info.st_mode & S_IFDIR)
                return true;
            else
                return false;
        }

        void comparePorts(Lib3MF::PImplicitPortIterator const& portIterator1,
                          Lib3MF::PImplicitPortIterator const& portIterator2,
                          bool ignoreReference = false)
        {
            ASSERT_EQ(portIterator1->Count(), portIterator2->Count());
            while(portIterator1->MoveNext())
            {
                EXPECT_TRUE(portIterator2->MoveNext());

                auto port1 = portIterator1->GetCurrent();
                auto port2 = portIterator2->GetCurrent();

                EXPECT_EQ(port1->GetIdentifier(), port2->GetIdentifier());
                EXPECT_EQ(port1->GetType(), port2->GetType());
                EXPECT_EQ(port1->GetDisplayName(), port2->GetDisplayName());
                if(!ignoreReference)
                {
                    EXPECT_EQ(port1->GetReference(), port2->GetReference());
                }
            }
            EXPECT_FALSE(portIterator2->MoveNext());
        }

        void compareFunctions(PImplicitFunction const& function1,
                              PImplicitFunction const& function2)
        {
            EXPECT_EQ(function1->GetDisplayName(), function2->GetDisplayName());
            EXPECT_EQ(function1->GetModelResourceID(),
                      function2->GetModelResourceID());

            auto nodeIterator1 = function1->GetNodes();
            auto nodeIterator2 = function2->GetNodes();
            ASSERT_EQ(nodeIterator1->Count(), nodeIterator2->Count());
            while(nodeIterator1->MoveNext())
            {
                EXPECT_TRUE(nodeIterator2->MoveNext());

                auto node1 = nodeIterator1->GetCurrent();
                auto node2 = nodeIterator2->GetCurrent();
                EXPECT_EQ(node1->GetIdentifier(), node2->GetIdentifier());
                EXPECT_EQ(node1->GetNodeType(), node2->GetNodeType());
                EXPECT_EQ(node1->GetDisplayName(), node2->GetDisplayName());

                if(node1->GetNodeType() == Lib3MF::eImplicitNodeType::Constant)
                {
                    EXPECT_EQ(node1->GetConstant(), node2->GetConstant());
                }

                comparePorts(node1->GetInputs(), node2->GetInputs(), false);
                comparePorts(node1->GetOutputs(), node2->GetOutputs(),
                             true);  // ignore reference
            }
            EXPECT_FALSE(nodeIterator2->MoveNext());

            auto inputs1 = function1->GetInputs();
            auto inputs2 = function2->GetInputs();
            ASSERT_EQ(inputs1->Count(), inputs2->Count());
            while(inputs1->MoveNext())
            {
                EXPECT_TRUE(inputs2->MoveNext());
                EXPECT_EQ(inputs1->GetCurrent()->GetDisplayName(),
                          inputs2->GetCurrent()->GetDisplayName());
                EXPECT_EQ(inputs1->GetCurrent()->GetIdentifier(),
                          inputs2->GetCurrent()->GetIdentifier());
                EXPECT_EQ(inputs1->GetCurrent()->GetType(),
                          inputs2->GetCurrent()->GetType());
            }
            EXPECT_FALSE(inputs2->MoveNext());

            auto outputs1 = function1->GetOutputs();
            auto outputs2 = function2->GetOutputs();
            ASSERT_EQ(outputs1->Count(), outputs2->Count());
            while(outputs1->MoveNext())
            {
                EXPECT_TRUE(outputs2->MoveNext());
                EXPECT_EQ(outputs1->GetCurrent()->GetDisplayName(),
                          outputs2->GetCurrent()->GetDisplayName());
                EXPECT_EQ(outputs1->GetCurrent()->GetIdentifier(),
                          outputs2->GetCurrent()->GetIdentifier());
                EXPECT_EQ(outputs1->GetCurrent()->GetType(),
                          outputs2->GetCurrent()->GetType());
            }
            EXPECT_FALSE(outputs2->MoveNext());
        }
    }  // namespace helper

    class Volumetric : public ::testing::Test
    {
       protected:
        virtual void SetUp()
        {
            if(!helper::directoryExists(OutFolder))
            {
                ASSERT_TRUE(CreateDir(OutFolder.c_str()))
                    << L"Could not create folder.";
            }
            model = wrapper->CreateModel();
            auto reader = model->QueryReader("3mf");
            reader->ReadFromFile(InFolder + "Pyramid.3mf");
            writer3MF = model->QueryWriter("3mf");
            writerSTL = model->QueryWriter("stl");
        }
        virtual void TearDown()
        {
            model.reset();
            writer3MF.reset();
            writerSTL.reset();
        }

        PModel model;
        PWriter writer3MF;
        PWriter writerSTL;

        static std::string InFolder;
        static std::string OutFolder;

        static void SetUpTestCase() { wrapper = CWrapper::loadLibrary(); }
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
        for(int i = 0; i < 11; i++)
        {
            std::string sNumber = "_";
            int k = i + 1;
            if(k < 10)
            {
                sNumber = sNumber + "0";
            }
            sNumber = sNumber + std::to_string(k);

            pImageStack->CreateSheetFromFile(
                i, "/volume/layer" + sNumber + ".png",
                InFolder + "img" + sNumber + ".png");
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

        // auto pVolumetricStack = model->AddVolumetricStack();
        // pVolumetricStack->AddDestinationChannel("channel", 0.0);
        // auto pLayer =
        // pVolumetricStack->AddLayer(wrapper->GetIdentityTransform(),
        // Lib3MF::eBlendMethod::Mix); auto pChannelSelector =
        // pLayer->AddChannelSelector(pImage3D.get(), "R", "channel");

        // auto theMesh = GetMesh();
        // auto volumeData = theMesh->VolumeData();

        // ASSERT_TRUE(volumeData->GetBoundary() == nullptr);
        // auto levelset =
        // volumeData->CreateNewLevelset(pVolumetricStack.get());

        // Lib3MF::sTransform sTransform = wrapper->GetIdentityTransform();
        // Lib3MF::sTransform sObtainedTransform = levelset->SetTransform();
        // ASSERT_FLOAT_EQ(sObtainedTransform.m_Fields[1][1], 1.0f);
        // sTransform.m_Fields[1][1] = 2.0;
        // levelset->SetTransform(sTransform);
        // sObtainedTransform = levelset->SetTransform();
        // ASSERT_FLOAT_EQ(sObtainedTransform.m_Fields[1][1],
        // sTransform.m_Fields[1][1]);

        // double dVal = 0.4;
        // levelset->SetSolidThreshold(dVal);
        // ASSERT_EQ(dVal, levelset->GetSolidThreshold());

        // std::string sChannelName("DoesNtWork");
        // ASSERT_SPECIFIC_THROW(levelset->SetChannel(sChannelName),
        // ELib3MFException); double dBackground = 0.0;
        // pVolumetricStack->GetDestinationChannel(0, sChannelName,
        // dBackground); levelset->SetChannel(sChannelName);
        // ASSERT_TRUE(levelset->GetChannel() == sChannelName);

        // Volumetric::writer3MF->WriteToFile(Volumetric::OutFolder +
        // "ColoredVolume.3mf");
    }

    TEST_F(Volumetric, VolumetricProperties)
    {
        auto pImage3D = SetupSheetsFromFile();

        auto fieldFromImage3D =
            model->AddScalarFieldFromImage3D(pImage3D.get());
        fieldFromImage3D->SetChannel(Lib3MF::eChannelName::Red);
        fieldFromImage3D->SetFilter(Lib3MF::eTextureFilter::Nearest);
        fieldFromImage3D->SetOffset(1.2);
        fieldFromImage3D->SetScale(.5);
        fieldFromImage3D->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap,
                                        Lib3MF::eTextureTileStyle::Mirror,
                                        Lib3MF::eTextureTileStyle::Clamp);

        auto constantField = model->AddScalarFieldConstant();
        ASSERT_EQ(constantField->GetValue(), 0.);
        double dValue = -0.2;
        constantField->SetValue(dValue);
        ASSERT_EQ(constantField->GetValue(), dValue);

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        std::string propertyName = "MyProperty";
        auto theProperty = volumeData->AddPropertyFromScalarField(
            propertyName, fieldFromImage3D.get());
        ASSERT_EQ(volumeData->GetPropertyCount(), 1);
        ASSERT_TRUE(theProperty->GetName() == propertyName);
        ASSERT_TRUE(theProperty->IsRequired());
        theProperty->SetIsRequired(false);
        ASSERT_FALSE(theProperty->IsRequired());

        auto theConstantProperty = volumeData->AddPropertyFromScalarField(
            "MyConstantProperty", constantField.get());
        ASSERT_EQ(volumeData->GetPropertyCount(), 2);

        writer3MF->WriteToFile(Volumetric::OutFolder + "MyProperties.3mf");

        {
            PModel ioModel = wrapper->CreateModel();
            PReader ioReader = ioModel->QueryReader("3mf");
            ioReader->ReadFromFile(Volumetric::OutFolder + "MyProperties.3mf");

            auto ioMeshObjects = ioModel->GetMeshObjects();
            ASSERT_EQ(ioMeshObjects->Count(), 1);
            ASSERT_TRUE(ioMeshObjects->MoveNext());
            auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
            auto ioVolumeData = ioMesh->VolumeData();
            CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

            PWriter ioWriter = ioModel->QueryWriter("3mf");
            ioWriter->WriteToFile(Volumetric::OutFolder +
                                  "MyPropertiesReOut.3mf");
        }

        volumeData->RemoveProperty(0);
        ASSERT_EQ(volumeData->GetPropertyCount(), 1);

        writer3MF->WriteToFile(Volumetric::OutFolder +
                               "MyProperties_after_deletion.3mf");
    }

    TEST_F(Volumetric, VolumetricCompositionScalar)
    {
        auto pImage3D = SetupSheetsFromFile();

        auto fieldFromImage3D =
            model->AddScalarFieldFromImage3D(pImage3D.get());

        auto constantField = model->AddScalarFieldConstant();
        ASSERT_EQ(constantField->GetValue(), 0.);
        double dValue = -0.2;
        constantField->SetValue(dValue);
        ASSERT_EQ(constantField->GetValue(), dValue);

        auto composedScalarField = model->AddScalarFieldComposed();
        composedScalarField->ScalarFieldReference1()->SetFieldResourceID(
            fieldFromImage3D->GetUniqueResourceID());
        composedScalarField->ScalarFieldReference2()->SetFieldResourceID(
            constantField->GetUniqueResourceID());
        composedScalarField->SetMethod(Lib3MF::eCompositionMethod::WeightedSum);
        composedScalarField->SetFactor1(1.0);
        composedScalarField->SetFactor2(-1.0);

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary =
            volumeData->CreateNewBoundary(composedScalarField.get());
        writer3MF->WriteToFile(Volumetric::OutFolder + "MyComposition.3mf");

        {
            PModel ioModel = wrapper->CreateModel();
            PReader ioReader = ioModel->QueryReader("3mf");
            ioReader->ReadFromFile(Volumetric::OutFolder + "MyComposition.3mf");

            auto scalarFieldIterator = ioModel->GetScalarFields();
            ASSERT_EQ(scalarFieldIterator->Count(), 3);

            ASSERT_TRUE(scalarFieldIterator->MoveNext());
            auto scalarField = scalarFieldIterator->GetCurrentScalarField();
            auto fieldFromImage3DReIn = ioModel->GetScalarFieldFromImage3DByID(
                scalarField->GetUniqueResourceID());
            CompareScalarFieldFromImage3D(ioModel, fieldFromImage3DReIn, model,
                                          fieldFromImage3D);

            ASSERT_TRUE(scalarFieldIterator->MoveNext());
            scalarField = scalarFieldIterator->GetCurrentScalarField();
            auto constantFieldReIn = ioModel->GetScalarFieldConstantByID(
                scalarField->GetUniqueResourceID());
            CompareScalarFieldConstant(constantFieldReIn, constantField);

            ASSERT_TRUE(scalarFieldIterator->MoveNext());
            scalarField = scalarFieldIterator->GetCurrentScalarField();
            auto composedScalarFieldReIn = ioModel->GetScalarFieldComposedByID(
                scalarField->GetUniqueResourceID());
            CompareScalarFieldComposed(ioModel, composedScalarFieldReIn, model,
                                       composedScalarField);
            ASSERT_FALSE(scalarFieldIterator->MoveNext());

            auto ioMeshObjects = ioModel->GetMeshObjects();
            ASSERT_EQ(ioMeshObjects->Count(), 1);
            ASSERT_TRUE(ioMeshObjects->MoveNext());
            auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
            auto ioVolumeData = ioMesh->VolumeData();
            CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

            PWriter ioWriter = ioModel->QueryWriter("3mf");
            ioWriter->WriteToFile(Volumetric::OutFolder +
                                  "MyCompositionReOut.3mf");
        }
    }

    TEST_F(Volumetric, VolumetricBoundary)
    {
        auto pImage3D = SetupSheetsFromFile();

        auto fieldFromImage3D =
            model->AddScalarFieldFromImage3D(pImage3D.get());
        fieldFromImage3D->SetChannel(Lib3MF::eChannelName::Red);

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary =
            volumeData->CreateNewBoundary(fieldFromImage3D.get());
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

            CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

            PWriter ioWriter = ioModel->QueryWriter("3mf");
            ioWriter->WriteToFile(Volumetric::OutFolder + "BoundaryReOut.3mf");
        }

        ASSERT_TRUE(volumeData->GetBoundary() != nullptr);
        volumeData->RemoveBoundary();
        ASSERT_TRUE(volumeData->GetBoundary() == nullptr);
    }

    TEST_F(Volumetric, VolumetricColor)
    {
        auto pImage3D = SetupSheetsFromFile();

        auto vectorFieldFromImage3D =
            model->AddVector3DFieldFromImage3D(pImage3D.get());
        vectorFieldFromImage3D->SetFilter(eTextureFilter::Nearest);
        vectorFieldFromImage3D->SetOffset(.1);
        vectorFieldFromImage3D->SetScale(0.9);
        vectorFieldFromImage3D->SetTileStyles(eTextureTileStyle::Clamp,
                                              eTextureTileStyle::Mirror,
                                              eTextureTileStyle::Wrap);

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theColour =
            volumeData->CreateNewColor(vectorFieldFromImage3D.get());

        writer3MF->WriteToFile(Volumetric::OutFolder + "Color.3mf");
        {
            PModel ioModel = wrapper->CreateModel();
            PReader ioReader = ioModel->QueryReader("3mf");
            ioReader->ReadFromFile(Volumetric::OutFolder + "Color.3mf");

            auto ioMeshObjects = ioModel->GetMeshObjects();
            ASSERT_EQ(ioMeshObjects->Count(), 1);
            ASSERT_TRUE(ioMeshObjects->MoveNext());
            auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
            auto ioVolumeData = ioMesh->VolumeData();
            CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

            auto vectorFields = ioModel->GetVector3DFields();
            ASSERT_TRUE(vectorFields->MoveNext());
            auto ioVectorFieldFromImage3D =
                vectorFields->GetCurrentVector3DField();
            CompareVector3DFields(ioModel, ioVectorFieldFromImage3D, model,
                                  vectorFieldFromImage3D);

            PWriter ioWriter = ioModel->QueryWriter("3mf");
            ioWriter->WriteToFile(Volumetric::OutFolder + "ColorReOut.3mf");
        }

        ASSERT_TRUE(volumeData->GetColor() != nullptr);
        volumeData->RemoveColor();
        ASSERT_TRUE(volumeData->GetColor() == nullptr);
    }

    TEST_F(Volumetric, VolumetricCompositionVector3D)
    {
        auto pImage3D = SetupSheetsFromFile();

        auto fieldFromImage3D =
            model->AddVector3DFieldFromImage3D(pImage3D.get());

        auto constantField = model->AddVector3DFieldConstant();
        ASSERT_EQ(constantField->GetValueX(), 0.);
        ASSERT_EQ(constantField->GetValueY(), 0.);
        ASSERT_EQ(constantField->GetValueZ(), 0.);
        double dValueX = -0.2;
        double dValueY = -0.3;
        double dValueZ = -0.4;
        constantField->SetValueX(dValueX);
        constantField->SetValueY(dValueY);
        constantField->SetValueZ(dValueZ);
        ASSERT_EQ(constantField->GetValueX(), dValueX);
        ASSERT_EQ(constantField->GetValueY(), dValueY);
        ASSERT_EQ(constantField->GetValueZ(), dValueZ);

        auto composedVector3DField = model->AddVector3DFieldComposed();
        composedVector3DField->Vector3DFieldReference1()->SetFieldResourceID(
            fieldFromImage3D->GetUniqueResourceID());
        composedVector3DField->Vector3DFieldReference2()->SetFieldResourceID(
            constantField->GetUniqueResourceID());
        composedVector3DField->SetMethod(
            Lib3MF::eCompositionMethod::WeightedSum);
        composedVector3DField->SetSpace(Lib3MF::eCompositionSpace::LinearColor);
        ASSERT_EQ(composedVector3DField->GetFactor1(), 1.);
        ASSERT_EQ(composedVector3DField->GetFactor2(), 1.);
        composedVector3DField->SetFactor1(1.5);
        composedVector3DField->SetFactor2(-1.0);
        ASSERT_EQ(composedVector3DField->GetFactor1(), 1.5);
        ASSERT_EQ(composedVector3DField->GetFactor2(), -1.);

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary =
            volumeData->CreateNewColor(composedVector3DField.get());
        writer3MF->WriteToFile(Volumetric::OutFolder +
                               "MyCompositionVector3D.3mf");

        {
            PModel ioModel = wrapper->CreateModel();
            PReader ioReader = ioModel->QueryReader("3mf");
            ioReader->ReadFromFile(Volumetric::OutFolder +
                                   "MyCompositionVector3D.3mf");

            auto vector3DFieldIterator = ioModel->GetVector3DFields();
            ASSERT_EQ(vector3DFieldIterator->Count(), 3);

            ASSERT_TRUE(vector3DFieldIterator->MoveNext());
            auto vector3DField =
                vector3DFieldIterator->GetCurrentVector3DField();
            auto fieldFromImage3DReIn =
                ioModel->GetVector3DFieldFromImage3DByID(
                    vector3DField->GetUniqueResourceID());
            CompareVector3DFieldFromImage3D(ioModel, fieldFromImage3DReIn,
                                            model, fieldFromImage3D);

            ASSERT_TRUE(vector3DFieldIterator->MoveNext());
            vector3DField = vector3DFieldIterator->GetCurrentVector3DField();
            auto constantFieldReIn = ioModel->GetVector3DFieldConstantByID(
                vector3DField->GetUniqueResourceID());
            CompareVector3DFieldConstant(constantFieldReIn, constantField);

            ASSERT_TRUE(vector3DFieldIterator->MoveNext());
            vector3DField = vector3DFieldIterator->GetCurrentVector3DField();
            auto composedVector3DFieldReIn =
                ioModel->GetVector3DFieldComposedByID(
                    vector3DField->GetUniqueResourceID());
            CompareVector3DFieldComposed(ioModel, composedVector3DFieldReIn,
                                         model, composedVector3DField);
            ASSERT_FALSE(vector3DFieldIterator->MoveNext());

            auto ioMeshObjects = ioModel->GetMeshObjects();
            ASSERT_EQ(ioMeshObjects->Count(), 1);
            ASSERT_TRUE(ioMeshObjects->MoveNext());
            auto ioMesh = ioMeshObjects->GetCurrentMeshObject();
            auto ioVolumeData = ioMesh->VolumeData();
            CompareVolumeData(ioModel, ioVolumeData, model, volumeData);

            PWriter ioWriter = ioModel->QueryWriter("3mf");
            ioWriter->WriteToFile(Volumetric::OutFolder +
                                  "MyCompositionVector3DReOut.3mf");
        }
    }

    TEST_F(Volumetric, AddFunction_NumberOfFunctionsIncreases)
    {
        auto resourceIteratorBefore = model->GetResources();
        EXPECT_EQ(resourceIteratorBefore->Count(), 2);
        auto functionIteratorBefore = model->GetFunctions();
        EXPECT_EQ(functionIteratorBefore->Count(), 0);
        PImplicitFunction newFunction = model->AddImplicitFunction();
        std::string const displayName = "test";
        newFunction->SetDisplayName(displayName);

        auto resourceIteratorAfter = model->GetResources();
        EXPECT_EQ(resourceIteratorAfter->Count(), 3);

        auto functionIterator = model->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        EXPECT_TRUE(functionIterator->MoveNext());
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetDisplayName(),
                  displayName);
        EXPECT_FALSE(functionIterator->MoveNext());
    }

    TEST_F(Volumetric, AddFunction_AddedFunctionAreWrittenTo3mfFile)
    {
        PImplicitFunction newFunction = model->AddImplicitFunction();
        std::string const displayName = "test";
        newFunction->SetDisplayName(displayName);
        auto const expectedResourceId = newFunction->GetModelResourceID();

        writer3MF->WriteToFile(Volumetric::OutFolder + "AddFunction.3mf");

        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "AddFunction.3mf");

        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        EXPECT_TRUE(functionIterator->MoveNext());
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetDisplayName(),
                  displayName);
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetModelResourceID(),
                  expectedResourceId);
        EXPECT_FALSE(functionIterator->MoveNext());
    }

    // Function with an "addition" node
    TEST_F(Volumetric,
           AddFunction_AddedFunctionWithAdditionNodeAreWrittenTo3mfFile)
    {
        PImplicitFunction newFunction = model->AddImplicitFunction();
        std::string const displayName = "test";
        newFunction->SetDisplayName(displayName);
        auto const expectedResourceId = newFunction->GetModelResourceID();

        // Add an addition node
        auto node = newFunction->AddNode(Lib3MF::eImplicitNodeType::Addition,
                                         "addition_1", "addition 1", "group_a");

        // Check if the node is added
        {
            auto nodeIterator = newFunction->GetNodes();
            ASSERT_EQ(nodeIterator->Count(), 1);
            EXPECT_TRUE(nodeIterator->MoveNext());
            EXPECT_EQ(nodeIterator->GetCurrent()->GetIdentifier(),
                      node->GetIdentifier());
        }
        // Write to file
        writer3MF->WriteToFile(Volumetric::OutFolder +
                               "AddFunctionWithAdditionNode.3mf");

        // Read from file
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder +
                               "AddFunctionWithAdditionNode.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        // // Check the addtion node
        EXPECT_TRUE(functionIterator->MoveNext());
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetModelResourceID(),
                  expectedResourceId);

        auto nodeIterator = functionIterator->GetCurrentFunction()->GetNodes();
        ASSERT_EQ(nodeIterator->Count(), 1);
        EXPECT_TRUE(nodeIterator->MoveNext());
        EXPECT_EQ(nodeIterator->GetCurrent()->GetNodeType(),
                  Lib3MF::eImplicitNodeType::Addition);
        EXPECT_EQ(nodeIterator->GetCurrent()->GetDisplayName(), "addition 1");

        EXPECT_FALSE(functionIterator->MoveNext());
    }

    // Function with multiple nodes and links between them
    TEST_F(Volumetric, AddFunction_MultipleNodesAndLinksAreWrittenTo3mfFile)
    {
        PImplicitFunction newFunction = model->AddImplicitFunction();
        std::string const displayName = "test";
        newFunction->SetDisplayName(displayName);
        auto const expectedResourceId = newFunction->GetModelResourceID();

        auto functionArgument = newFunction->AddInput(
            "pos", "position", Lib3MF::eImplicitPortType::Vector);

        // Add some nodes
        auto addNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Addition,
                                 "addition_1", "addition 1", "group_a");
        auto subNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Subtraction,
                                 "substraction_1", "substraction 1", "group_a");
        auto mulNode = newFunction->AddNode(
            Lib3MF::eImplicitNodeType::Multiplication, "multiplication_1",
            "multiplication 3", "group_a");

        // Add some links
        newFunction->AddLinkByNames("addition_1.result", "substraction_1.A");
        // Alternative way to add links
        mulNode->FindInput("A")->SetReference("addition_1.result");

        auto output =
            newFunction->AddOutput("shape", "signed distance to the surface",
                                   Lib3MF::eImplicitPortType::Vector);
        output->SetReference("substraction_1.result");

        // Write to file
        writer3MF->WriteToFile(Volumetric::OutFolder +
                               "AddFunctionWithMultipleNodesAndLinks.3mf");

        // Read from file
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder +
                               "AddFunctionWithMultipleNodesAndLinks.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        // Check the nodes
        EXPECT_TRUE(functionIterator->MoveNext());
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetModelResourceID(),
                  expectedResourceId);

        // Compare the functions
        helper::compareFunctions(newFunction,
                                 functionIterator->GetCurrentFunction());
    }

    // Function with constant node
    TEST_F(Volumetric, ImplicitSphere)
    {
        PImplicitFunction newFunction = model->AddImplicitFunction();
        newFunction->SetDisplayName("sphere");
        auto const expectedResourceId = newFunction->GetModelResourceID();

        auto functionArgument = newFunction->AddInput(
            "pos", "position", Lib3MF::eImplicitPortType::Vector);

        // Add some nodes
        auto constMatrixNode = newFunction->AddNode(
            Lib3MF::eImplicitNodeType::ConstMat, "matrix_1",
            "unused example matrix", "group_a");

        // clang-format off
        constMatrixNode->SetMatrix(
          {1.23456, 2.34567, 3.45678, 4.56789,
           5.67890, 6.78901, 7.89012, 8.90123, 
           9.01234, 10.12345, 11.23456, 12.34567, 
           13.45678, 14.56789, 15.67890, 16.78901});
        // clang-format on

        auto constNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Constant, "radius",
                                 "radius of the spehere", "group_a");
        constNode->SetConstant(1.23456);

        auto constVecNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::ConstVec,
                                 "vector_1", "translation vector", "group_a");
        constVecNode->SetVector({1.23456, 2.34567, 3.45678});
        constVecNode->FindOutput("vector")->SetType(
            Lib3MF::eImplicitPortType::Vector);

        auto subNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Subtraction,
                                 "translate_1", "Translation", "group_a");

        auto subInputA = subNode->FindInput("A");
        auto subInputB = subNode->FindInput("B");
        subInputA->SetType(Lib3MF::eImplicitPortType::Vector);
        subInputB->SetType(Lib3MF::eImplicitPortType::Vector);
        subInputA->SetReference("inputs.pos");
        subInputB->SetReference("vector_1.vector");
        subNode->FindOutput("result")->SetType(
            Lib3MF::eImplicitPortType::Vector);

        auto distanceToSpehereNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Length,
                                 "distance_1", "distance to sphere", "group_a");

        distanceToSpehereNode->FindInput("A")->SetType(
            Lib3MF::eImplicitPortType::Vector);
        distanceToSpehereNode->FindInput("A")->SetReference(
            "translate_1.result");

        // Substract radius from distance
        auto subNode2 =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Subtraction,
                                 "distance_2", "distance to sphere", "group_a");
        subNode2->FindInput("A")->SetReference("distance_1.result");

        auto output =
            newFunction->AddOutput("shape", "signed distance to the surface",
                                   Lib3MF::eImplicitPortType::Scalar);
        output->SetReference("distance_2.result");

        // add the function as volume data defining a boundary to the mesh
        /* auto scalarField = model->AddScalarFieldFunction();
         ASSERT_NE(scalarField.get(), nullptr);
         scalarField->SetFunction(newFunction.get());
        


         scalarField->SetOutput("shape");
         auto theMesh = GetMesh();
         auto volumeData = theMesh->VolumeData();
         auto theBoundary = volumeData->CreateNewBoundary(scalarField.get());*/

        // Write to file
        writer3MF->WriteToFile(Volumetric::OutFolder + "ImplicitSphere.3mf");

        // // Read from file
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "ImplicitSphere.3mf");

        // // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        // Check the nodes
        EXPECT_TRUE(functionIterator->MoveNext());
        EXPECT_EQ(functionIterator->GetCurrentFunction()->GetModelResourceID(),
                  expectedResourceId);

        // Compare the functions
        helper::compareFunctions(newFunction,
                                 functionIterator->GetCurrentFunction());
    }

    TEST_F(Volumetric, ImplicitGyroid)
    {
        PImplicitFunction newFunction = model->AddImplicitFunction();
        newFunction->SetDisplayName("gyroid");

        auto functionArgument = newFunction->AddInput(
            "pos", "position", Lib3MF::eImplicitPortType::Vector);

        auto decomposePos =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::DecomposeVector,
                                 "decomposePos", "decompose pos", "group_a");

        decomposePos->FindInput("vector")->SetType(
            Lib3MF::eImplicitPortType::Vector);
        newFunction->AddLinkByNames("inputs.pos", "decomposePos.vector");

        auto composeYZX =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::ComposeVector,
                                 "composeYZX", "compose yzx", "group_a");
        composeYZX->FindOutput("vector")->SetType(
            Lib3MF::eImplicitPortType::Vector);

        newFunction->AddLinkByNames("decomposePos.z", "composeYZX.y");
        newFunction->AddLinkByNames("decomposePos.y", "composeYZX.x");
        newFunction->AddLinkByNames("decomposePos.x", "composeYZX.z");

        // Add the necessay nodes and links for the gyroid (dot(sin(pos),
        // cos(composeYZX))
        auto sinNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Sinus,
                                            "sin", "sin", "group_a");
        sinNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
        sinNode->FindOutput("result")->SetType(
            Lib3MF::eImplicitPortType::Vector);
        newFunction->AddLinkByNames("inputs.pos", "sin.A");

        auto cosNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Cosinus,
                                            "cos", "cos", "group_a");
        cosNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
        cosNode->FindOutput("result")->SetType(
            Lib3MF::eImplicitPortType::Vector);
        newFunction->AddLinkByNames("composeYZX.vector", "cos.A");

        auto dotNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Dot,
                                            "dot", "dot", "group_a");

        dotNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
        dotNode->FindInput("B")->SetType(Lib3MF::eImplicitPortType::Vector);
        newFunction->AddLinkByNames("sin.result", "dot.A");
        newFunction->AddLinkByNames("cos.result", "dot.B");

        auto output =
            newFunction->AddOutput("shape", "signed distance to the surface",
                                   Lib3MF::eImplicitPortType::Scalar);

        output->SetReference("dot.result");

        // write to file
        writer3MF->WriteToFile(Volumetric::OutFolder + "ImplicitGyroid.3mf");

        // read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "ImplicitGyroid.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        // Check the nodes
        EXPECT_TRUE(functionIterator->MoveNext());

        // Compare the functions
        helper::compareFunctions(newFunction,
                                 functionIterator->GetCurrentFunction());
    }

    // Create a shell with the distance to a mesh provided by the mesh node
    TEST_F(Volumetric, ShellFromMesh)
    {
        // Create a function
        PImplicitFunction newFunction = model->AddImplicitFunction();
        newFunction->SetDisplayName("shell");

        auto functionArgument = newFunction->AddInput(
            "pos", "position", Lib3MF::eImplicitPortType::Vector);

        auto resourceNode = newFunction->AddNode(
            Lib3MF::eImplicitNodeType::Resource, "meshResource",
            "mesh resource", "group_shell");

        auto mesh = GetMesh();

        resourceNode->SetResource(mesh.get());
        resourceNode->FindOutput("value")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);

        // Create a mesh node
        auto meshNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Mesh,
                                             "mesh", "mesh", "group_shell");

        meshNode->FindInput("pos")->SetType(Lib3MF::eImplicitPortType::Vector);
        meshNode->FindInput("mesh")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);
        newFunction->AddLinkByNames("inputs.pos", "mesh.pos");

        newFunction->AddLinkByNames("meshResource.value", "mesh.mesh");

        auto absNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Abs,
                                            "abs", "abs", "group_shell");

        newFunction->AddLinkByNames("mesh.distance", "abs.A");

        auto constScalarNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Constant,
                                 "thickness", "thickness", "group_shell");

        constScalarNode->SetConstant(2.);

        auto subtractionNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Subtraction,
                                 "subtraction", "subtraction", "group_shell");

        subtractionNode->FindInput("A")->SetType(
            Lib3MF::eImplicitPortType::Scalar);

        newFunction->AddLinkByNames("abs.result", "subtraction.A");
        newFunction->AddLinkByNames("thickness.value", "subtraction.B");

        auto output =
            newFunction->AddOutput("shape", "signed distance to the surface",
                                   Lib3MF::eImplicitPortType::Scalar);

        output->SetReference("subtraction.result");

        // write to file
        writer3MF->WriteToFile(Volumetric::OutFolder + "ShellFromMesh.3mf");

        // read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "ShellFromMesh.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        EXPECT_TRUE(functionIterator->MoveNext());

        // Compare the functions
        helper::compareFunctions(newFunction,
                                 functionIterator->GetCurrentFunction());
    }

    TEST_F(Volumetric, SubFunction)
    {
        // Create a gyroid function
        PImplicitFunction gyroidFunction = model->AddImplicitFunction();
        {
            gyroidFunction->SetDisplayName("gyroid");

            auto functionArgument = gyroidFunction->AddInput(
                "pos", "position", Lib3MF::eImplicitPortType::Vector);

            auto decomposePos = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::DecomposeVector, "decomposePos",
                "decompose pos", "group_a");

            decomposePos->FindInput("vector")->SetType(
                Lib3MF::eImplicitPortType::Vector);
            gyroidFunction->AddLinkByNames("inputs.pos", "decomposePos.vector");

            auto composeYZX = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::ComposeVector, "composeYZX",
                "compose yzx", "group_a");
            composeYZX->FindOutput("vector")->SetType(
                Lib3MF::eImplicitPortType::Vector);

            gyroidFunction->AddLinkByNames("decomposePos.z", "composeYZX.y");
            gyroidFunction->AddLinkByNames("decomposePos.y", "composeYZX.x");
            gyroidFunction->AddLinkByNames("decomposePos.x", "composeYZX.z");

            // Add the necessay nodes and links for the gyroid (dot(sin(pos),
            // cos(composeYZX))
            auto sinNode = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::Sinus, "sin", "sin", "group_a");
            sinNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
            sinNode->FindOutput("result")->SetType(
                Lib3MF::eImplicitPortType::Vector);
            gyroidFunction->AddLinkByNames("inputs.pos", "sin.A");

            auto cosNode = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::Cosinus, "cos", "cos", "group_a");
            cosNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
            cosNode->FindOutput("result")->SetType(
                Lib3MF::eImplicitPortType::Vector);
            gyroidFunction->AddLinkByNames("composeYZX.vector", "cos.A");

            auto dotNode = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::Dot, "dot", "dot", "group_a");

            dotNode->FindInput("A")->SetType(Lib3MF::eImplicitPortType::Vector);
            dotNode->FindInput("B")->SetType(Lib3MF::eImplicitPortType::Vector);
            gyroidFunction->AddLinkByNames("sin.result", "dot.A");
            gyroidFunction->AddLinkByNames("cos.result", "dot.B");

            auto output = gyroidFunction->AddOutput(
                "shape", "signed distance to the surface",
                Lib3MF::eImplicitPortType::Scalar);

            output->SetReference("dot.result");
        }

        PImplicitFunction newFunction = model->AddImplicitFunction();
        newFunction->SetDisplayName("shell");

        auto functionArgument = newFunction->AddInput(
            "pos", "position", Lib3MF::eImplicitPortType::Vector);

        auto resourceNode = newFunction->AddNode(
            Lib3MF::eImplicitNodeType::Resource, "meshResource",
            "mesh resource", "group_shell");

        auto mesh = GetMesh();

        resourceNode->SetResource(mesh.get());
        resourceNode->FindOutput("value")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);

        // Create a mesh node
        auto meshNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Mesh,
                                             "mesh", "mesh", "group_shell");

        meshNode->FindInput("pos")->SetType(Lib3MF::eImplicitPortType::Vector);
        meshNode->FindInput("mesh")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);
        newFunction->AddLinkByNames("inputs.pos", "mesh.pos");

        newFunction->AddLinkByNames("meshResource.value", "mesh.mesh");

        auto absNode = newFunction->AddNode(Lib3MF::eImplicitNodeType::Abs,
                                            "abs", "abs", "group_shel l");

        newFunction->AddLinkByNames("mesh.distance", "abs.A");

        auto constScalarNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Constant,
                                 "thickness", "thickness", "group_shell");

        constScalarNode->SetConstant(2.);

        auto subtractionNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Subtraction,
                                 "subtraction", "subtraction", "group_shell");

        subtractionNode->FindInput("A")->SetType(
            Lib3MF::eImplicitPortType::Scalar);

        newFunction->AddLinkByNames("abs.result", "subtraction.A");
        newFunction->AddLinkByNames("thickness.value", "subtraction.B");

        auto output =
            newFunction->AddOutput("shape", "signed distance to the surface",
                                   Lib3MF::eImplicitPortType::Scalar);

        auto gyroidNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::FunctionCall,
                                 "gyroid", "gyroid", "group_gyroid");

        auto funcitionIdNode = newFunction->AddNode(
            Lib3MF::eImplicitNodeType::Resource, "gyroidID",
            "function resource", "group_gyroid");

        funcitionIdNode->FindOutput("value")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);

        funcitionIdNode->SetResource(gyroidFunction.get());

        auto functionInput = gyroidNode->FindInput("functionID");
        ASSERT_TRUE(functionInput);

        functionInput->SetType(Lib3MF::eImplicitPortType::ResourceID);

        newFunction->AddLinkByNames("gyroidID.value", "gyroid.functionID");

        // Currently you have to add the inputs and outputs of the called
        // function manually. We should automate this.
        gyroidNode->AddInput("pos", "position")
            ->SetType(Lib3MF::eImplicitPortType::Vector);

        gyroidNode->AddOutput("shape", "signed distance to the surface")
            ->SetType(Lib3MF::eImplicitPortType::Scalar);

        newFunction->AddLinkByNames("inputs.pos", "gyroid.pos");

        auto maxNode =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::Max, "max",
                                 "max - intersection", "group_shell");

        newFunction->AddLinkByNames("gyroid.shape", "max.A");
        newFunction->AddLinkByNames("subtraction.result", "max.B");

        // output->SetReference("subtraction.result");
        output->SetReference("max.result");

        // write to file
        writer3MF->WriteToFile(Volumetric::OutFolder +
                               "ShellWithGyroidSubfunction.3mf");

        // read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder +
                               "ShellWithGyroidSubfunction.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 2);

        EXPECT_TRUE(functionIterator->MoveNext());

        // Compare the functions
        helper::compareFunctions(gyroidFunction,
                                 functionIterator->GetCurrentFunction());
        EXPECT_TRUE(functionIterator->MoveNext());
        helper::compareFunctions(newFunction,
                                 functionIterator->GetCurrentFunction());
    }

    // Test for loading a 3MF file with an image stack
    TEST_F(Volumetric, LoadImageStack)
    {
        // Read from file
        PModel model = wrapper->CreateModel();
        PReader reader = model->QueryReader("3mf");
        reader->ReadFromFile(Volumetric::InFolder + "Boundary.3mf");

        // check if the file was read correctly
        auto readerWarnings = reader->GetWarningCount();
        ASSERT_EQ(readerWarnings, 0);

        // Check the image stack
        auto image3dIterator = model->GetImage3Ds();
        ASSERT_EQ(image3dIterator->Count(), 1);

        while(image3dIterator->MoveNext())
        {
                auto image3d = image3dIterator->GetCurrentImage3D();
                ASSERT_TRUE(image3d);
                EXPECT_TRUE(image3d->IsImageStack());
        }
    }

    TEST_F(Volumetric, CreateImplicitWithFunctionFromImage3D)
    {
        // Create model
        model = wrapper->CreateModel();

        // Create a new image stack
        auto pImage3D = SetupSheetsFromFile();

        auto funcFromImage3d = model->AddFunctionFromImage3D(pImage3D.get());
        ASSERT_TRUE(funcFromImage3d);
        funcFromImage3d->SetDisplayName("function from image3d");

        auto const funcImg3dId = funcFromImage3d->GetUniqueResourceID();
        funcFromImage3d->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap,
                                       Lib3MF::eTextureTileStyle::Wrap,
                                       Lib3MF::eTextureTileStyle::Wrap);

        // Write to file
        writer3MF = model->QueryWriter("3mf");
        writer3MF->WriteToFile(Volumetric::OutFolder + "FunctionFromImage3D.3mf");
    }

}  // namespace Lib3MF
