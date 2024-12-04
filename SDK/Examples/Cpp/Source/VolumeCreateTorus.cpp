#include "lib3mf_implicit.hpp"
#include <iostream>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    auto torusFunction = model->AddImplicitFunction();
    torusFunction->SetDisplayName("Torus");

    auto input = torusFunction->AddInput("pos", "Position", Lib3MF::eImplicitPortType::Vector);
    auto lengthNode = torusFunction->AddLengthNode("length", "Length", "group");
    torusFunction->AddLink(input, lengthNode->GetInputA());

    auto radiusNode = torusFunction->AddConstantNode("radius", "Radius", "group");
    radiusNode->SetConstant(10.0);

    auto subtractNode = torusFunction->AddSubtractionNode("subtract", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "Subtract Radius", "group");
    torusFunction->AddLink(lengthNode->GetOutputResult(), subtractNode->GetInputA());
    torusFunction->AddLink(radiusNode->GetOutputValue(), subtractNode->GetInputB());

    auto output = torusFunction->AddOutput("shape", "Torus Shape", Lib3MF::eImplicitPortType::Scalar);
    torusFunction->AddLink(subtractNode->GetOutputResult(), output);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("Torus.3mf");

    std::cout << "Saved Torus.3mf" << std::endl;
    return 0;
}