#include "lib3mf_implicit.hpp"
#include <iostream>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    auto funcA = model->AddImplicitFunction();
    auto funcB = model->AddImplicitFunction();
    funcA->SetDisplayName("FunctionA");
    funcB->SetDisplayName("FunctionB");

    auto inputA = funcA->AddInput("x", "Position X", Lib3MF::eImplicitPortType::Scalar);
    auto sinNode = funcA->AddSinNode("sinA", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "Sine Node", "group");
    funcA->AddLink(inputA, sinNode->GetInputA());
    funcA->AddOutput("outputA", "Output A", Lib3MF::eImplicitPortType::Scalar);

    auto inputB = funcB->AddInput("y", "Position Y", Lib3MF::eImplicitPortType::Scalar);
    auto cosNode = funcB->AddCosNode("cosB", Lib3MF::eImplicitNodeConfiguration::ScalarToScalar, "Cosine Node", "group");
    funcB->AddLink(inputB, cosNode->GetInputA());
    funcB->AddOutput("outputB", "Output B", Lib3MF::eImplicitPortType::Scalar);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("CombinedFunctions.3mf");

    std::cout << "Saved CombinedFunctions.3mf" << std::endl;
    return 0;
}