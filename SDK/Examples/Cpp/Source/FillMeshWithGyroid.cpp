#include <iostream>

#include "lib3mf_implicit.hpp"

Lib3MF::PImplicitFunction createGyroidFunction(Lib3MF::CModel& model)
{
    Lib3MF::PImplicitFunction gyroidFunction = model.AddImplicitFunction();
    gyroidFunction->SetDisplayName("gyroid");

    auto inputPos = gyroidFunction->AddInput("pos", "position",
                                             Lib3MF::eImplicitPortType::Vector);

    auto decomposePos = gyroidFunction->AddDecomposeVectorNode(
        "decomposePos", "decompose pos", "group_a");
    gyroidFunction->AddLink(inputPos, decomposePos->GetInputA());

    auto composeYZX = gyroidFunction->AddComposeVectorNode(
        "composeYZX", "compose yzx", "group_a");
    gyroidFunction->AddLink(decomposePos->GetOutputZ(),
                            composeYZX->GetInputY());
    gyroidFunction->AddLink(decomposePos->GetOutputY(),
                            composeYZX->GetInputX());
    gyroidFunction->AddLink(decomposePos->GetOutputX(),
                            composeYZX->GetInputZ());

    auto sinNode = gyroidFunction->AddSinNode(
        "sin", Lib3MF::eImplicitNodeConfiguration::VectorToVector, "sinus",
        "group_a");
    gyroidFunction->AddLink(inputPos, sinNode->GetInputA());

    auto cosNode = gyroidFunction->AddCosNode(
        "cos", Lib3MF::eImplicitNodeConfiguration::VectorToVector, "cosinus",
        "group_a");
    gyroidFunction->AddLink(composeYZX->GetOutputResult(),
                            cosNode->GetInputA());

    auto dotNode = gyroidFunction->AddDotNode("dot", "dot product", "group_a");
    gyroidFunction->AddLink(sinNode->GetOutputResult(), dotNode->GetInputA());
    gyroidFunction->AddLink(cosNode->GetOutputResult(), dotNode->GetInputB());

    auto absNode = gyroidFunction->AddAbsNode(
        "abs", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "abs",
        "group_a");
    gyroidFunction->AddLink(dotNode->GetOutputResult(), absNode->GetInputA());

    auto substractionNode = gyroidFunction->AddSubtractionNode(
        "sub", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "substract",
        "group_a");

    auto halfThicknessNode =
        gyroidFunction->AddConstantNode("half_thickness", "hafl of the thickness", "group_a");
    halfThicknessNode->SetConstant(0.4);

    gyroidFunction->AddLink(absNode->GetOutputResult(),
                            substractionNode->GetInputA());
    gyroidFunction->AddLink(halfThicknessNode->GetOutputValue(),
                            substractionNode->GetInputB());

    auto output =
        gyroidFunction->AddOutput("shape", "signed distance to the surface",
                                  Lib3MF::eImplicitPortType::Scalar);
    gyroidFunction->AddLink(substractionNode->GetOutputResult(), output);

    return gyroidFunction;
}

Lib3MF::PMeshObject createBoxAsMesh(Lib3MF::CModel& model, float sizeX,
                                    float sizeY, float sizeZ)
{
    Lib3MF::PMeshObject meshObject = model.AddMeshObject();
    meshObject->SetName("Box");

    // Create mesh structure of a cube
    std::vector<Lib3MF::sPosition> vertices(8);
    std::vector<Lib3MF::sTriangle> triangles(12);

    // Manually create vertices
    vertices[0] = {0.0f, 0.0f, 0.0f};
    vertices[1] = {sizeX, 0.0f, 0.0f};
    vertices[2] = {sizeX, sizeY, 0.0f};
    vertices[3] = {0.0f, sizeY, 0.0f};
    vertices[4] = {0.0f, 0.0f, sizeZ};
    vertices[5] = {sizeX, 0.0f, sizeZ};
    vertices[6] = {sizeX, sizeY, sizeZ};
    vertices[7] = {0.0f, sizeY, sizeZ};

    // Manually create triangles
    triangles[0] = {0, 1, 2};
    triangles[1] = {0, 2, 3};
    triangles[2] = {4, 5, 6};
    triangles[3] = {4, 6, 7};
    triangles[4] = {0, 1, 5};
    triangles[5] = {0, 5, 4};
    triangles[6] = {1, 2, 6};
    triangles[7] = {1, 6, 5};
    triangles[8] = {2, 3, 7};
    triangles[9] = {2, 7, 6};
    triangles[10] = {3, 0, 4};
    triangles[11] = {3, 4, 7};

    // Add vertices and triangles to the mesh object
    meshObject->SetGeometry(vertices, triangles);

    return meshObject;
}

int main()
{
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    // First we create the resourees we want to use
    //==================================================================================================
    // In this example we just create a box as a mesh, but you could as well
    // extend it to load a 3mf file that already contains a mesh object
    auto box = createBoxAsMesh(*model, 50.0f, 23.0f, 45.0f);

    auto gyroidFunction = createGyroidFunction(*model);

    // Now we create the gyroid surface as level set, with the mesh definining
    // the evaluation domain
    auto theLevelSet = model->AddLevelSet();
    theLevelSet->SetFunction(gyroidFunction.get());
    theLevelSet->SetChannelName("shape");
    theLevelSet->SetMesh(box.get());
    theLevelSet->SetMeshBBoxOnly(
        true);  // Treating the mesh as a bounding box only makes the evaluation
                // faster. Disable this for abritrary meshes.

    // Add a build item for the level set
    model->AddBuildItem(theLevelSet.get(), wrapper->GetIdentityTransform());

    // write the model to a file
    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("GyroidBox.3mf");
}