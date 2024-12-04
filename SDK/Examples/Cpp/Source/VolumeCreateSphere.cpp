#include "lib3mf_implicit.hpp"
#include <iostream>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    auto newFunction = model->AddImplicitFunction();
    newFunction->SetDisplayName("Sphere");

    auto input = newFunction->AddInput("pos", "position", Lib3MF::eImplicitPortType::Vector);
    auto constantNode = newFunction->AddConstantNode("radius", "Sphere Radius", "group");
    constantNode->SetConstant(15.0);

    auto lengthNode = newFunction->AddLengthNode("distance", "Distance to sphere", "group");
    newFunction->AddLink(input, lengthNode->GetInputA());

    auto subtractNode = newFunction->AddSubtractionNode("offset", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "Offset Radius", "group");
    newFunction->AddLink(lengthNode->GetOutputResult(), subtractNode->GetInputA());
    newFunction->AddLink(constantNode->GetOutputValue(), subtractNode->GetInputB());

    auto output = newFunction->AddOutput("shape", "Signed Distance Field", Lib3MF::eImplicitPortType::Scalar);
    newFunction->AddLink(subtractNode->GetOutputResult(), output);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("ImplicitSphere.3mf");

    std::cout << "Saved ImplicitSphere.3mf" << std::endl;
    return 0;
}