#include "lib3mf_implicit.hpp"
#include <iostream>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    auto newFunction = model->AddImplicitFunction();
    newFunction->SetDisplayName("Cylinder");

    auto input = newFunction->AddInput("pos", "Position", Lib3MF::eImplicitPortType::Vector);
    auto cylinderNode = newFunction->AddLengthNode("cylinderNode", "Length", "group");
    newFunction->AddLink(input, cylinderNode->GetInputA());

    auto output = newFunction->AddOutput("shape", "Cylinder Shape", Lib3MF::eImplicitPortType::Scalar);
    newFunction->AddLink(cylinderNode->GetOutputResult(), output);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("Cylinder.3mf");

    std::cout << "Saved Cylinder.3mf" << std::endl;
    return 0;
}