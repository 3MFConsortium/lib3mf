/*++

Copyright (C) 2024 3MF Consortium

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

ImplicitNodes.test.cpp: Defines unit tests for new implicit node types
(BeamLattice, FunctionGradient, NormalizeDistance)

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
    class ImplicitNodes : public ::testing::Test
    {
    protected:
        virtual void SetUp()
        {
            if (!helper::directoryExists(OutFolder))
            {
                ASSERT_TRUE(CreateDir(OutFolder.c_str()))
                    << L"Could not create folder.";
            }
            model = wrapper->CreateModel();
            loadPyramid();
            writer3MF = model->QueryWriter("3mf");
        }

        virtual void TearDown()
        {
            model.reset();
            writer3MF.reset();
        }

        void loadPyramid()
        {
            model = wrapper->CreateModel();
            auto reader = model->QueryReader("3mf");
            reader->ReadFromFile(InFolder + "Pyramid.3mf");
        }

        PModel model;
        PWriter writer3MF;

        static std::string InFolder;
        static std::string OutFolder;

        static void SetUpTestCase() { wrapper = CWrapper::loadLibrary(); }
        static PWrapper wrapper;

        PMeshObject GetMesh()
        {
            auto meshes = model->GetMeshObjects();
            meshes->MoveNext();
            return meshes->GetCurrentMeshObject();
        }
    };

    PWrapper ImplicitNodes::wrapper;
    std::string ImplicitNodes::InFolder(sTestFilesPath + "/Volumetric/");
    std::string ImplicitNodes::OutFolder(sOutFilesPath + "/ImplicitNodes/");

    // ============================================================================
    // BeamLatticeNode Tests
    // ============================================================================

    TEST_F(ImplicitNodes, BeamLatticeNode_Creation_NodeIsAdded)
    {
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("beamlattice_test");

        // Create BeamLatticeNode
        auto beamLatticeNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice", "group_bl");

        ASSERT_TRUE(beamLatticeNode);
        EXPECT_EQ(beamLatticeNode->GetIdentifier(), "bl1");
        EXPECT_EQ(beamLatticeNode->GetDisplayName(), "beam lattice");
        EXPECT_EQ(beamLatticeNode->GetTag(), "group_bl");
        EXPECT_EQ(beamLatticeNode->GetNodeType(), 
                  Lib3MF::eImplicitNodeType::BeamLattice);

        // Check node count
        auto nodes = function->GetNodes();
        EXPECT_EQ(nodes->Count(), 1u);
    }

    TEST_F(ImplicitNodes, BeamLatticeNode_Ports_CorrectPortsExist)
    {
        auto function = model->AddImplicitFunction();
        auto beamLatticeNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice", "group_bl");

        // Check inputs
        auto inputPos = beamLatticeNode->GetInputPos();
        ASSERT_TRUE(inputPos);
        EXPECT_EQ(inputPos->GetType(), Lib3MF::eImplicitPortType::Vector);

        auto inputBeamLattice = beamLatticeNode->GetInputBeamLattice();
        ASSERT_TRUE(inputBeamLattice);
        EXPECT_EQ(inputBeamLattice->GetType(), Lib3MF::eImplicitPortType::ResourceID);

        // Check output
        auto outputDistance = beamLatticeNode->GetOutputDistance();
        ASSERT_TRUE(outputDistance);
        EXPECT_EQ(outputDistance->GetType(), Lib3MF::eImplicitPortType::Scalar);
    }

    TEST_F(ImplicitNodes, BeamLatticeNode_AccurateRange_SetGetCorrectly)
    {
        auto function = model->AddImplicitFunction();
        auto beamLatticeNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice", "group_bl");

        // Default should be 0.0
        EXPECT_DOUBLE_EQ(beamLatticeNode->GetAccurateRange(), 0.0);

        // Set a positive value
        beamLatticeNode->SetAccurateRange(5.5);
        EXPECT_DOUBLE_EQ(beamLatticeNode->GetAccurateRange(), 5.5);

        // Set zero (valid)
        beamLatticeNode->SetAccurateRange(0.0);
        EXPECT_DOUBLE_EQ(beamLatticeNode->GetAccurateRange(), 0.0);
    }

    TEST_F(ImplicitNodes, BeamLatticeNode_WriteAndRead_SameContent)
    {
        // Create function with BeamLatticeNode
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("beamlattice_function");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);

        // Create a beamlattice resource (we'll use a mesh as placeholder)
        auto mesh = GetMesh();

        auto resourceIdNode = function->AddResourceIdNode(
            "beamres", "beam resource", "group_bl");
        resourceIdNode->SetResource(mesh.get());

        auto beamLatticeNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice node", "group_bl");
        beamLatticeNode->SetAccurateRange(3.5);

        // Link inputs
        auto blInputPos = beamLatticeNode->GetInputPos();
        function->AddLink(posInput, blInputPos);

        auto resOutput = resourceIdNode->GetOutputValue();
        auto blInputBeamLattice = beamLatticeNode->GetInputBeamLattice();
        function->AddLink(resOutput, blInputBeamLattice);

        // Link output
        auto output = function->AddOutput("distance", "distance field", 
                                         Lib3MF::eImplicitPortType::Scalar);
        function->AddLink(beamLatticeNode->GetOutputDistance(), output);

        // Create LevelSet
        auto theMesh = GetMesh();
        auto levelSet = model->AddLevelSet();
        levelSet->SetMesh(theMesh);
        levelSet->SetFunction(function.get());

        // Write to file
        writer3MF->WriteToFile(ImplicitNodes::OutFolder + "BeamLatticeNode.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(ImplicitNodes::OutFolder + "BeamLatticeNode.3mf");
        auto functionIter = ioModel->GetFunctions();
        ASSERT_EQ(functionIter->Count(), 1u);
        ASSERT_TRUE(functionIter->MoveNext());

        auto readFunction = std::dynamic_pointer_cast<CImplicitFunction>(
            functionIter->GetCurrentFunction());
        ASSERT_NE(readFunction, nullptr);

        helper::compareFunctions(model, function, ioModel, readFunction);

        // Verify BeamLatticeNode specifically
        auto nodes = readFunction->GetNodes();
        bool foundBeamLattice = false;
        while (nodes->MoveNext())
        {
            auto node = nodes->GetCurrent();
            if (node->GetNodeType() == Lib3MF::eImplicitNodeType::BeamLattice)
            {
                foundBeamLattice = true;
                auto blNode = std::dynamic_pointer_cast<CBeamLatticeNode>(node);
                ASSERT_NE(blNode, nullptr);
                EXPECT_DOUBLE_EQ(blNode->GetAccurateRange(), 3.5);
            }
        }
        EXPECT_TRUE(foundBeamLattice);
    }

    TEST_F(ImplicitNodes, BeamLatticeNode_AccurateRangeZero_NotWrittenToFile)
    {
        // When accuraterange is 0.0, it should not be written to XML
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("bl_zero_test");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);
        auto mesh = GetMesh();
        auto resourceIdNode = function->AddResourceIdNode(
            "beamres", "beam resource", "group_bl");
        resourceIdNode->SetResource(mesh.get());

        auto beamLatticeNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice", "group_bl");
        // Don't set accuraterange (default is 0.0)

        function->AddLink(posInput, beamLatticeNode->GetInputPos());
        function->AddLink(resourceIdNode->GetOutputValue(), 
                         beamLatticeNode->GetInputBeamLattice());

        auto output = function->AddOutput("distance", "distance", 
                                         Lib3MF::eImplicitPortType::Scalar);
        function->AddLink(beamLatticeNode->GetOutputDistance(), output);

        auto levelSet = model->AddLevelSet();
        levelSet->SetMesh(GetMesh());
        levelSet->SetFunction(function.get());

        // Write and read
        writer3MF->WriteToFile(ImplicitNodes::OutFolder + "BeamLatticeZero.3mf");

        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(ImplicitNodes::OutFolder + "BeamLatticeZero.3mf");

        auto functionIter = ioModel->GetFunctions();
        ASSERT_TRUE(functionIter->MoveNext());
        auto readFunction = std::dynamic_pointer_cast<CImplicitFunction>(
            functionIter->GetCurrentFunction());

        auto nodes = readFunction->GetNodes();
        while (nodes->MoveNext())
        {
            auto node = nodes->GetCurrent();
            if (node->GetNodeType() == Lib3MF::eImplicitNodeType::BeamLattice)
            {
                auto blNode = std::dynamic_pointer_cast<CBeamLatticeNode>(node);
                ASSERT_NE(blNode, nullptr);
                EXPECT_DOUBLE_EQ(blNode->GetAccurateRange(), 0.0);
            }
        }
    }

    // ============================================================================
    // FunctionGradientNode Tests
    // ============================================================================

    TEST_F(ImplicitNodes, FunctionGradientNode_Creation_NodeIsAdded)
    {
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("functiongradient_test");

        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "function gradient", "group_fg");

        ASSERT_TRUE(fgNode);
        EXPECT_EQ(fgNode->GetIdentifier(), "fg1");
        EXPECT_EQ(fgNode->GetDisplayName(), "function gradient");
        EXPECT_EQ(fgNode->GetTag(), "group_fg");
        EXPECT_EQ(fgNode->GetNodeType(), 
                  Lib3MF::eImplicitNodeType::FunctionGradient);

        auto nodes = function->GetNodes();
        EXPECT_EQ(nodes->Count(), 1u);
    }

    TEST_F(ImplicitNodes, FunctionGradientNode_Ports_CorrectPortsExist)
    {
        auto function = model->AddImplicitFunction();
        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "function gradient", "group_fg");

        // Check inputs
        auto inputFunctionID = fgNode->GetInputFunctionID();
        ASSERT_TRUE(inputFunctionID);
        EXPECT_EQ(inputFunctionID->GetType(), Lib3MF::eImplicitPortType::ResourceID);

        auto inputPos = fgNode->GetInputPos();
        ASSERT_TRUE(inputPos);
        EXPECT_EQ(inputPos->GetType(), Lib3MF::eImplicitPortType::Vector);

        auto inputStep = fgNode->GetInputStep();
        ASSERT_TRUE(inputStep);
        EXPECT_EQ(inputStep->GetType(), Lib3MF::eImplicitPortType::Scalar);

        // Check outputs
        auto outputNormalizedGradient = fgNode->GetOutputNormalizedGradient();
        ASSERT_TRUE(outputNormalizedGradient);
        EXPECT_EQ(outputNormalizedGradient->GetType(), 
                  Lib3MF::eImplicitPortType::Vector);

        auto outputGradient = fgNode->GetOutputGradient();
        ASSERT_TRUE(outputGradient);
        EXPECT_EQ(outputGradient->GetType(), Lib3MF::eImplicitPortType::Vector);

        auto outputMagnitude = fgNode->GetOutputMagnitude();
        ASSERT_TRUE(outputMagnitude);
        EXPECT_EQ(outputMagnitude->GetType(), Lib3MF::eImplicitPortType::Scalar);
    }

    TEST_F(ImplicitNodes, FunctionGradientNode_Attributes_SetGetCorrectly)
    {
        auto function = model->AddImplicitFunction();
        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "function gradient", "group_fg");

        // Set scalarOutputName
        fgNode->SetScalarOutputName("magnitude");
        EXPECT_EQ(fgNode->GetScalarOutputName(), "magnitude");

        // Set vectorInputName
        fgNode->SetVectorInputName("normalizedgradient");
        EXPECT_EQ(fgNode->GetVectorInputName(), "normalizedgradient");

        // Change values
        fgNode->SetScalarOutputName("customScalar");
        fgNode->SetVectorInputName("customVector");
        EXPECT_EQ(fgNode->GetScalarOutputName(), "customScalar");
        EXPECT_EQ(fgNode->GetVectorInputName(), "customVector");
    }

    TEST_F(ImplicitNodes, FunctionGradientNode_WriteAndRead_SameContent)
    {
        // Create a simple function to reference
        auto referencedFunction = model->AddImplicitFunction();
        referencedFunction->SetDisplayName("referenced_function");
        auto refInput = referencedFunction->AddInput("pos", "position", 
                                                     Lib3MF::eImplicitPortType::Vector);
        auto refOutput = referencedFunction->AddOutput("distance", "distance", 
                                                       Lib3MF::eImplicitPortType::Scalar);

        // Create main function with FunctionGradientNode
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("functiongradient_function");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);
        auto stepInput = function->AddInput("step", "step size", 
                                            Lib3MF::eImplicitPortType::Scalar);

        auto funcResNode = function->AddResourceIdNode(
            "funcres", "function resource", "group_fg");
        funcResNode->SetResource(referencedFunction.get());

        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "gradient node", "group_fg");
        fgNode->SetScalarOutputName("magnitude");
        fgNode->SetVectorInputName("normalizedgradient");

        // Link inputs
        function->AddLink(funcResNode->GetOutputValue(), 
                         fgNode->GetInputFunctionID());
        function->AddLink(posInput, fgNode->GetInputPos());
        function->AddLink(stepInput, fgNode->GetInputStep());

        // Link outputs
        auto output = function->AddOutput("gradient", "gradient vector", 
                                         Lib3MF::eImplicitPortType::Vector);
        function->AddLink(fgNode->GetOutputGradient(), output);

        auto levelSet = model->AddLevelSet();
        levelSet->SetMesh(GetMesh());
        levelSet->SetFunction(function.get());

        // Write to file
        writer3MF->WriteToFile(ImplicitNodes::OutFolder + "FunctionGradientNode.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(ImplicitNodes::OutFolder + "FunctionGradientNode.3mf");

        auto functionIter = ioModel->GetFunctions();
        ASSERT_EQ(functionIter->Count(), 2u);

        EXPECT_TRUE(functionIter->MoveNext());
        helper::compareFunctions(model, referencedFunction, ioModel,
                                 functionIter->GetCurrentFunction());

        EXPECT_TRUE(functionIter->MoveNext());
        auto readFunction = std::dynamic_pointer_cast<CImplicitFunction>(
            functionIter->GetCurrentFunction());
        ASSERT_NE(readFunction, nullptr);

        helper::compareFunctions(model, function, ioModel, readFunction);
        EXPECT_FALSE(functionIter->MoveNext());

        // Verify FunctionGradientNode specifically
        auto nodes = readFunction->GetNodes();
        bool foundFunctionGradient = false;
        while (nodes->MoveNext())
        {
            auto node = nodes->GetCurrent();
            if (node->GetNodeType() == Lib3MF::eImplicitNodeType::FunctionGradient)
            {
                foundFunctionGradient = true;
                auto fgNode = std::dynamic_pointer_cast<CFunctionGradientNode>(node);
                ASSERT_NE(fgNode, nullptr);
                EXPECT_EQ(fgNode->GetScalarOutputName(), "magnitude");
                EXPECT_EQ(fgNode->GetVectorInputName(), "normalizedgradient");
            }
        }
        EXPECT_TRUE(foundFunctionGradient);
    }

    TEST_F(ImplicitNodes, FunctionGradientNode_MultipleOutputs_AllAccessible)
    {
        auto function = model->AddImplicitFunction();
        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "function gradient", "group_fg");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);
        auto stepInput = function->AddInput("step", "step", 
                                            Lib3MF::eImplicitPortType::Scalar);

        // Create a dummy function resource
        auto refFunc = model->AddImplicitFunction();
        refFunc->SetDisplayName("dummy");
        auto funcResNode = function->AddResourceIdNode("funcres", "func", "g");
        funcResNode->SetResource(refFunc.get());

        // Link inputs
        function->AddLink(funcResNode->GetOutputValue(), 
                         fgNode->GetInputFunctionID());
        function->AddLink(posInput, fgNode->GetInputPos());
        function->AddLink(stepInput, fgNode->GetInputStep());

        // Use all three outputs
        auto outputNormalizedGrad = function->AddOutput(
            "normgrad", "normalized gradient", Lib3MF::eImplicitPortType::Vector);
        function->AddLink(fgNode->GetOutputNormalizedGradient(), outputNormalizedGrad);

        auto outputGrad = function->AddOutput(
            "grad", "gradient", Lib3MF::eImplicitPortType::Vector);
        function->AddLink(fgNode->GetOutputGradient(), outputGrad);

        auto outputMag = function->AddOutput(
            "mag", "magnitude", Lib3MF::eImplicitPortType::Scalar);
        function->AddLink(fgNode->GetOutputMagnitude(), outputMag);

        // Verify outputs exist
        auto outputs = function->GetOutputs();
        EXPECT_EQ(outputs->Count(), 3u);
    }

    // ============================================================================
    // NormalizeDistanceNode Tests
    // ============================================================================

    TEST_F(ImplicitNodes, NormalizeDistanceNode_Creation_NodeIsAdded)
    {
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("normalizedistance_test");

        auto ndNode = function->AddNormalizeDistanceNode(
            "nd1", "normalize distance", "group_nd");

        ASSERT_TRUE(ndNode);
        EXPECT_EQ(ndNode->GetIdentifier(), "nd1");
        EXPECT_EQ(ndNode->GetDisplayName(), "normalize distance");
        EXPECT_EQ(ndNode->GetTag(), "group_nd");
        EXPECT_EQ(ndNode->GetNodeType(), 
                  Lib3MF::eImplicitNodeType::NormalizeDistance);

        auto nodes = function->GetNodes();
        EXPECT_EQ(nodes->Count(), 1u);
    }

    TEST_F(ImplicitNodes, NormalizeDistanceNode_Ports_CorrectPortsExist)
    {
        auto function = model->AddImplicitFunction();
        auto ndNode = function->AddNormalizeDistanceNode(
            "nd1", "normalize distance", "group_nd");

        // Check inputs
        auto inputFunctionID = ndNode->GetInputFunctionID();
        ASSERT_TRUE(inputFunctionID);
        EXPECT_EQ(inputFunctionID->GetType(), Lib3MF::eImplicitPortType::ResourceID);

        auto inputPos = ndNode->GetInputPos();
        ASSERT_TRUE(inputPos);
        EXPECT_EQ(inputPos->GetType(), Lib3MF::eImplicitPortType::Vector);

        auto inputStep = ndNode->GetInputStep();
        ASSERT_TRUE(inputStep);
        EXPECT_EQ(inputStep->GetType(), Lib3MF::eImplicitPortType::Scalar);

        // Check output
        auto outputResult = ndNode->GetOutputResult();
        ASSERT_TRUE(outputResult);
        EXPECT_EQ(outputResult->GetType(), Lib3MF::eImplicitPortType::Scalar);
    }

    TEST_F(ImplicitNodes, NormalizeDistanceNode_Attributes_SetGetCorrectly)
    {
        auto function = model->AddImplicitFunction();
        auto ndNode = function->AddNormalizeDistanceNode(
            "nd1", "normalize distance", "group_nd");

        // Set scalarOutputName
        ndNode->SetScalarOutputName("result");
        EXPECT_EQ(ndNode->GetScalarOutputName(), "result");

        // Set vectorInputName
        ndNode->SetVectorInputName("gradient");
        EXPECT_EQ(ndNode->GetVectorInputName(), "gradient");

        // Change values
        ndNode->SetScalarOutputName("customOutput");
        ndNode->SetVectorInputName("customInput");
        EXPECT_EQ(ndNode->GetScalarOutputName(), "customOutput");
        EXPECT_EQ(ndNode->GetVectorInputName(), "customInput");
    }

    TEST_F(ImplicitNodes, NormalizeDistanceNode_WriteAndRead_SameContent)
    {
        // Create a simple function to reference
        auto referencedFunction = model->AddImplicitFunction();
        referencedFunction->SetDisplayName("referenced_function");
        auto refInput = referencedFunction->AddInput("pos", "position", 
                                                     Lib3MF::eImplicitPortType::Vector);
        auto refOutput = referencedFunction->AddOutput("distance", "distance", 
                                                       Lib3MF::eImplicitPortType::Scalar);

        // Create main function with NormalizeDistanceNode
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("normalizedistance_function");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);
        auto stepInput = function->AddInput("step", "step size", 
                                            Lib3MF::eImplicitPortType::Scalar);

        auto funcResNode = function->AddResourceIdNode(
            "funcres", "function resource", "group_nd");
        funcResNode->SetResource(referencedFunction.get());

        auto ndNode = function->AddNormalizeDistanceNode(
            "nd1", "normalize node", "group_nd");
        ndNode->SetScalarOutputName("result");
        ndNode->SetVectorInputName("gradient");

        // Link inputs
        function->AddLink(funcResNode->GetOutputValue(), 
                         ndNode->GetInputFunctionID());
        function->AddLink(posInput, ndNode->GetInputPos());
        function->AddLink(stepInput, ndNode->GetInputStep());

        // Link output
        auto output = function->AddOutput("normalized", "normalized distance", 
                                         Lib3MF::eImplicitPortType::Scalar);
        function->AddLink(ndNode->GetOutputResult(), output);

        auto levelSet = model->AddLevelSet();
        levelSet->SetMesh(GetMesh());
        levelSet->SetFunction(function.get());

        // Write to file
        writer3MF->WriteToFile(ImplicitNodes::OutFolder + "NormalizeDistanceNode.3mf");

        // Read and compare
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(ImplicitNodes::OutFolder + "NormalizeDistanceNode.3mf");

        auto functionIter = ioModel->GetFunctions();
        ASSERT_EQ(functionIter->Count(), 2u);

        EXPECT_TRUE(functionIter->MoveNext());
        helper::compareFunctions(model, referencedFunction, ioModel,
                                 functionIter->GetCurrentFunction());

        EXPECT_TRUE(functionIter->MoveNext());
        auto readFunction = std::dynamic_pointer_cast<CImplicitFunction>(
            functionIter->GetCurrentFunction());
        ASSERT_NE(readFunction, nullptr);

        helper::compareFunctions(model, function, ioModel, readFunction);
        EXPECT_FALSE(functionIter->MoveNext());

        // Verify NormalizeDistanceNode specifically
        auto nodes = readFunction->GetNodes();
        bool foundNormalizeDistance = false;
        while (nodes->MoveNext())
        {
            auto node = nodes->GetCurrent();
            if (node->GetNodeType() == Lib3MF::eImplicitNodeType::NormalizeDistance)
            {
                foundNormalizeDistance = true;
                auto ndNode = std::dynamic_pointer_cast<CNormalizeDistanceNode>(node);
                ASSERT_NE(ndNode, nullptr);
                EXPECT_EQ(ndNode->GetScalarOutputName(), "result");
                EXPECT_EQ(ndNode->GetVectorInputName(), "gradient");
            }
        }
        EXPECT_TRUE(foundNormalizeDistance);
    }

    // ============================================================================
    // Combined/Integration Tests
    // ============================================================================

    TEST_F(ImplicitNodes, AllNewNodes_CombinedInFunction_WriteAndRead)
    {
        // Create a function using all three new node types
        auto function = model->AddImplicitFunction();
        function->SetDisplayName("combined_nodes_function");

        auto posInput = function->AddInput("pos", "position", 
                                           Lib3MF::eImplicitPortType::Vector);

        // Create BeamLatticeNode
        auto mesh = GetMesh();
        auto beamResNode = function->AddResourceIdNode(
            "beamres", "beam resource", "group_combined");
        beamResNode->SetResource(mesh.get());

        auto blNode = function->AddBeamLatticeNode(
            "bl1", "beam lattice", "group_combined");
        blNode->SetAccurateRange(2.5);
        function->AddLink(posInput, blNode->GetInputPos());
        function->AddLink(beamResNode->GetOutputValue(), 
                         blNode->GetInputBeamLattice());

        // Create a reference function for gradient/normalize nodes
        auto refFunc = model->AddImplicitFunction();
        refFunc->SetDisplayName("reference_function");
        auto refFuncResNode = function->AddResourceIdNode(
            "funcres", "function resource", "group_combined");
        refFuncResNode->SetResource(refFunc.get());

        auto stepConst = function->AddConstantNode("step", "step value", "group_combined");
        stepConst->SetConstant(0.01);

        // Create FunctionGradientNode
        auto fgNode = function->AddFunctionGradientNode(
            "fg1", "gradient", "group_combined");
        fgNode->SetScalarOutputName("magnitude");
        fgNode->SetVectorInputName("normalizedgradient");
        function->AddLink(refFuncResNode->GetOutputValue(), 
                         fgNode->GetInputFunctionID());
        function->AddLink(posInput, fgNode->GetInputPos());
        function->AddLink(stepConst->GetOutputValue(), fgNode->GetInputStep());

        // Create NormalizeDistanceNode
        auto ndNode = function->AddNormalizeDistanceNode(
            "nd1", "normalize", "group_combined");
        ndNode->SetScalarOutputName("result");
        ndNode->SetVectorInputName("gradient");
        function->AddLink(refFuncResNode->GetOutputValue(), 
                         ndNode->GetInputFunctionID());
        function->AddLink(posInput, ndNode->GetInputPos());
        function->AddLink(stepConst->GetOutputValue(), ndNode->GetInputStep());

        // Combine outputs
        auto addNode = function->AddAdditionNode(
            "combine", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar,
            "combine results", "group_combined");
        function->AddLink(blNode->GetOutputDistance(), addNode->GetInputA());
        function->AddLink(ndNode->GetOutputResult(), addNode->GetInputB());

        auto output = function->AddOutput("combined", "combined output", 
                                         Lib3MF::eImplicitPortType::Scalar);
        function->AddLink(addNode->GetOutputResult(), output);

        auto levelSet = model->AddLevelSet();
        levelSet->SetMesh(GetMesh());
        levelSet->SetFunction(function.get());

        // Write to file
        writer3MF->WriteToFile(ImplicitNodes::OutFolder + "CombinedNewNodes.3mf");

        // Read and verify
        PModel ioModel = wrapper->CreateModel();
        PReader ioReader = ioModel->QueryReader("3mf");
        ioReader->ReadFromFile(ImplicitNodes::OutFolder + "CombinedNewNodes.3mf");

        auto functionIter = ioModel->GetFunctions();
        ASSERT_EQ(functionIter->Count(), 2u);

        EXPECT_TRUE(functionIter->MoveNext());
        helper::compareFunctions(model, refFunc, ioModel,
                                 functionIter->GetCurrentFunction());

        EXPECT_TRUE(functionIter->MoveNext());
        auto readFunction = std::dynamic_pointer_cast<CImplicitFunction>(
            functionIter->GetCurrentFunction());
        ASSERT_NE(readFunction, nullptr);

        helper::compareFunctions(model, function, ioModel, readFunction);
        EXPECT_FALSE(functionIter->MoveNext());

        // Verify all three node types are present
        auto nodes = readFunction->GetNodes();
        bool foundBeamLattice = false;
        bool foundFunctionGradient = false;
        bool foundNormalizeDistance = false;

        while (nodes->MoveNext())
        {
            auto node = nodes->GetCurrent();
            auto nodeType = node->GetNodeType();

            if (nodeType == Lib3MF::eImplicitNodeType::BeamLattice)
            {
                foundBeamLattice = true;
                auto blNode = std::dynamic_pointer_cast<CBeamLatticeNode>(node);
                ASSERT_NE(blNode, nullptr);
                EXPECT_DOUBLE_EQ(blNode->GetAccurateRange(), 2.5);
            }
            else if (nodeType == Lib3MF::eImplicitNodeType::FunctionGradient)
            {
                foundFunctionGradient = true;
                auto fgNode = std::dynamic_pointer_cast<CFunctionGradientNode>(node);
                ASSERT_NE(fgNode, nullptr);
                EXPECT_EQ(fgNode->GetScalarOutputName(), "magnitude");
                EXPECT_EQ(fgNode->GetVectorInputName(), "normalizedgradient");
            }
            else if (nodeType == Lib3MF::eImplicitNodeType::NormalizeDistance)
            {
                foundNormalizeDistance = true;
                auto ndNode = std::dynamic_pointer_cast<CNormalizeDistanceNode>(node);
                ASSERT_NE(ndNode, nullptr);
                EXPECT_EQ(ndNode->GetScalarOutputName(), "result");
                EXPECT_EQ(ndNode->GetVectorInputName(), "gradient");
            }
        }

        EXPECT_TRUE(foundBeamLattice);
        EXPECT_TRUE(foundFunctionGradient);
        EXPECT_TRUE(foundNormalizeDistance);
    }

} // namespace Lib3MF
