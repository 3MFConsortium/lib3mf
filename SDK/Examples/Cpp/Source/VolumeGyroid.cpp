#include "lib3mf_implicit.hpp"
#include <iostream>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    auto gyroidFunction = model->AddImplicitFunction();
    gyroidFunction->SetDisplayName("Gyroid");

    auto input = gyroidFunction->AddInput("pos", "position", Lib3MF::eImplicitPortType::Vector);
    auto sinNode = gyroidFunction->AddSinNode("sin", Lib3MF::eImplicitNodeConfiguration::VectorToVector, "Sine of Position", "group");
    auto cosNode = gyroidFunction->AddCosNode("cos", Lib3MF::eImplicitNodeConfiguration::VectorToVector, "Cosine of Transformed Position", "group");

    auto dotNode = gyroidFunction->AddDotNode("dot", "Dot Product", "group");
    gyroidFunction->AddLink(input, sinNode->GetInputA());
    gyroidFunction->AddLink(cosNode->GetOutputResult(), dotNode->GetInputB());
    gyroidFunction->AddLink(sinNode->GetOutputResult(), dotNode->GetInputA());

    auto output = gyroidFunction->AddOutput("shape", "Signed Distance Field", Lib3MF::eImplicitPortType::Scalar);
    gyroidFunction->AddLink(dotNode->GetOutputResult(), output);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("Gyroid.3mf");

    std::cout << "Saved Gyroid.3mf" << std::endl;
    return 0;
}