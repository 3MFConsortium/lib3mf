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
            loadPyramid();
            writer3MF = model->QueryWriter("3mf");
            writerSTL = model->QueryWriter("stl");
        }
        virtual void TearDown()
        {
            model.reset();
            writer3MF.reset();
            writerSTL.reset();
        }

        void loadPyramid()
        {
            model = wrapper->CreateModel();
            auto reader = model->QueryReader("3mf");
            reader->ReadFromFile(InFolder + "Pyramid.3mf");
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

        PImplicitFunction currentImplicitFunction = std::dynamic_pointer_cast<CImplicitFunction>(functionIterator->GetCurrentFunction());
        ASSERT_NE(currentImplicitFunction, nullptr);

        auto nodeIterator = currentImplicitFunction->GetNodes();
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

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary = volumeData->CreateNewBoundary(newFunction.get());

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
        helper::compareFunctions(model, newFunction, ioModel,
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

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary = volumeData->CreateNewBoundary(newFunction.get());

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
        helper::compareFunctions(model, newFunction, ioModel,
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

        decomposePos->FindInput("A")->SetType(
            Lib3MF::eImplicitPortType::Vector);
        newFunction->AddLinkByNames("inputs.pos", "decomposePos.A");

        auto composeYZX =
            newFunction->AddNode(Lib3MF::eImplicitNodeType::ComposeVector,
                                 "composeYZX", "compose yzx", "group_a");
        composeYZX->FindOutput("result")->SetType(
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
        newFunction->AddLinkByNames("composeYZX.result", "cos.A");

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

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theBoundary = volumeData->CreateNewBoundary(newFunction.get());

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
        helper::compareFunctions(model, newFunction, ioModel,
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
        helper::compareFunctions(model,newFunction, ioModel,
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

            decomposePos->FindInput("A")->SetType(
                Lib3MF::eImplicitPortType::Vector);
            gyroidFunction->AddLinkByNames("inputs.pos", "decomposePos.A");

            auto composeYZX = gyroidFunction->AddNode(
                Lib3MF::eImplicitNodeType::ComposeVector, "composeYZX",
                "compose yzx", "group_a");
            composeYZX->FindOutput("result")->SetType(
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
            gyroidFunction->AddLinkByNames("composeYZX.result", "cos.A");

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
        helper::compareFunctions(model, gyroidFunction, ioModel,
                                 functionIterator->GetCurrentFunction());
        EXPECT_TRUE(functionIterator->MoveNext());
        helper::compareFunctions(model, newFunction, ioModel,
                                 functionIterator->GetCurrentFunction());
    }

    TEST_F(Volumetric, CreateAndLoad_FunctionFromImage3dColor_SameContent)
    {
        // Create model and load pyramid
        model = wrapper->CreateModel();
        auto reader = model->QueryReader("3mf");
        reader->ReadFromFile(InFolder + "Cube.3mf");

        // Create a new image stack
        auto pImage3D = SetupSheetsFromFile();

        auto funcFromImage3d = model->AddFunctionFromImage3D(pImage3D.get());
        ASSERT_TRUE(funcFromImage3d);
        funcFromImage3d->SetDisplayName("function from image3d");

        auto const funcImg3dId = funcFromImage3d->GetUniqueResourceID();
        funcFromImage3d->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap,
                                       Lib3MF::eTextureTileStyle::Clamp,
                                       Lib3MF::eTextureTileStyle::Mirror);


        // Create a new implicit function
        auto implicitFunction = model->AddImplicitFunction();
        ASSERT_TRUE(implicitFunction);
        implicitFunction->SetDisplayName("implicit function");

        // Add pos as input
        auto posInput = implicitFunction->AddInput("pos", "position", Lib3MF::eImplicitPortType::Vector);
        ASSERT_TRUE(posInput);

        // Add color as output
        auto colorOutput = implicitFunction->AddOutput("color", "color", Lib3MF::eImplicitPortType::Vector);
        ASSERT_TRUE(colorOutput);

        //Call a node to call the function from image3d
        auto functionCallNode =
            implicitFunction->AddNode(Lib3MF::eImplicitNodeType::FunctionCall,
                                      "functionCall", "functionCall",
                                      "group_functionCall");

        auto functionIdNode = implicitFunction->AddNode(
            Lib3MF::eImplicitNodeType::Resource, "functionID",
            "function resource", "group_functionCall");

        functionIdNode->FindOutput("value")->SetType(
            Lib3MF::eImplicitPortType::ResourceID);

        functionIdNode->SetResource(funcFromImage3d.get());

        auto functionInput = functionCallNode->FindInput("functionID");
        ASSERT_TRUE(functionInput);

        functionInput->SetType(Lib3MF::eImplicitPortType::ResourceID);

        implicitFunction->AddLinkByNames("functionID.value",
                                         "functionCall.functionID");

        // Currently you have to add the inputs and outputs of the called
        // function manually. We should automate this.

        functionCallNode->AddInput("pos", "position")
            ->SetType(Lib3MF::eImplicitPortType::Vector);

        functionCallNode->AddOutput("color", "signed distance to the surface")
            ->SetType(Lib3MF::eImplicitPortType::Vector);

        implicitFunction->AddLinkByNames("inputs.pos", "functionCall.pos");
        implicitFunction->AddLinkByNames("functionCall.color", "outputs.color");

        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        auto theColor = volumeData->CreateNewColor(implicitFunction.get());

        // Set transformation
        auto transformation = helper::ComputeTransformFromMeshCoordinatesToUVW(theMesh);
        theColor->SetTransform(transformation);
    
        // Write to file
        writer3MF = model->QueryWriter("3mf");
        writer3MF->WriteToFile(Volumetric::OutFolder + "FunctionFromImage3D_Color.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "FunctionFromImage3D_Color.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 2);

        EXPECT_TRUE(functionIterator->MoveNext());
        auto functionFromFile = functionIterator->GetCurrentFunction();
        ASSERT_TRUE(functionFromFile);

        PFunctionFromImage3D funcFromFileAsImage3D =
            std::dynamic_pointer_cast<CFunctionFromImage3D>(functionFromFile);

        // Compare the functions
        helper::compareFunctions(model, funcFromImage3d, ioModel, functionFromFile);
    }

    TEST_F(Volumetric, CreateAndLoad_FunctionFromImage3dAddBoundary_SameContent)
    {
        model = wrapper->CreateModel();
        auto reader = model->QueryReader("3mf");
        reader->ReadFromFile(InFolder + "Cube.3mf");

        // Create a new image stack
        auto pImage3D = SetupSheetsFromFile();

        auto funcFromImage3d = model->AddFunctionFromImage3D(pImage3D.get());
        ASSERT_TRUE(funcFromImage3d);
        funcFromImage3d->SetDisplayName("function from image3d");

        auto const funcImg3dId = funcFromImage3d->GetUniqueResourceID();
        funcFromImage3d->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap,
                                       Lib3MF::eTextureTileStyle::Clamp,
                                       Lib3MF::eTextureTileStyle::Mirror);

                                       // add volume data
        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        
        // Add boundary
        auto boundary = volumeData->CreateNewBoundary(funcFromImage3d.get());
        boundary->SetTransform(helper::ComputeTransformFromMeshCoordinatesToUVW(theMesh));
        boundary->SetSolidThreshold(0.25f);
        boundary->SetOutputName("r");
        
        // Todo: Define output name

        // Write to file
        writer3MF = model->QueryWriter("3mf");
        writer3MF->WriteToFile(Volumetric::OutFolder + "Boundary.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "Boundary.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        EXPECT_TRUE(functionIterator->MoveNext());
        auto functionFromFile = functionIterator->GetCurrentFunction();
        ASSERT_TRUE(functionFromFile);

        PFunctionFromImage3D funcFromFileAsImage3D =
            std::dynamic_pointer_cast<CFunctionFromImage3D>(functionFromFile);

        // Compare the functions
        helper::compareFunctions(model, funcFromImage3d, ioModel, functionFromFile);

        // Check the boundary
        auto boundaryFromFile = volumeData->GetBoundary();
        ASSERT_TRUE(boundaryFromFile);
        EXPECT_EQ(boundaryFromFile->GetFunctionResourceID(), funcImg3dId);
        helper::CompareTransforms(boundary->GetTransform(), boundaryFromFile->GetTransform());
        EXPECT_EQ(boundary->GetSolidThreshold(), boundaryFromFile->GetSolidThreshold());
    }

    TEST_F(Volumetric, CreateAndLoad_FunctionFromImage3dAddProperty_SameContent)
    {
        model = wrapper->CreateModel();
        auto reader = model->QueryReader("3mf");
        reader->ReadFromFile(InFolder + "Cube.3mf");

        // Create a new image stack
        auto pImage3D = SetupSheetsFromFile();

        auto funcFromImage3d = model->AddFunctionFromImage3D(pImage3D.get());
        ASSERT_TRUE(funcFromImage3d);
        funcFromImage3d->SetDisplayName("function from image3d");

        auto const funcImg3dId = funcFromImage3d->GetUniqueResourceID();
        funcFromImage3d->SetTileStyles(Lib3MF::eTextureTileStyle::Wrap,
                                       Lib3MF::eTextureTileStyle::Clamp,
                                       Lib3MF::eTextureTileStyle::Mirror);

        // add volume data
        auto theMesh = GetMesh();
        auto volumeData = theMesh->VolumeData();
        
        // Add property
        auto property = volumeData->AddPropertyFromFunction("Temp", funcFromImage3d.get());
        property->SetTransform(helper::ComputeTransformFromMeshCoordinatesToUVW(theMesh));
        std::string const outputName = "g";
        property->SetOutputName(outputName);

        // Write to file
        writer3MF = model->QueryWriter("3mf");
        writer3MF->WriteToFile(Volumetric::OutFolder + "Property.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(Volumetric::OutFolder + "Property.3mf");

        // Check the function
        auto functionIterator = ioModel->GetFunctions();
        ASSERT_EQ(functionIterator->Count(), 1);

        EXPECT_TRUE(functionIterator->MoveNext());
        auto functionFromFile = functionIterator->GetCurrentFunction();
        ASSERT_TRUE(functionFromFile);

        PFunctionFromImage3D funcFromFileAsImage3D =
            std::dynamic_pointer_cast<CFunctionFromImage3D>(functionFromFile);

        // Compare the functions
        helper::compareFunctions(model, funcFromImage3d, ioModel, functionFromFile);

        // Check the property
        ASSERT_EQ(volumeData->GetPropertyCount(), 1);
        auto propertyFromFile = volumeData->GetProperty(0);
        ASSERT_TRUE(propertyFromFile);
        EXPECT_EQ(propertyFromFile->GetFunctionResourceID(), funcImg3dId);
        helper::CompareTransforms(property->GetTransform(), propertyFromFile->GetTransform());
        EXPECT_EQ(property->GetOutputName(), outputName);
        EXPECT_EQ(property->GetName(), propertyFromFile->GetName());
    }

}  // namespace Lib3MF
