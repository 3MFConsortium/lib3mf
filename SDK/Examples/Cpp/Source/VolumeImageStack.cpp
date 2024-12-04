#include "lib3mf_implicit.hpp"
#include <iostream>
#include <filesystem>

int main() {
    Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();
    auto model = wrapper->CreateModel();

    constexpr int layers = 11;

    auto pImageStack = model->AddImageStack(821, 819, layers);

    // Resolve the path to the volumetricImages folder relative to the source file
    std::string sVolumeImagesFolder = VOLUME_IMAGES_PATH;
    std::filesystem::path sourceDir(sVolumeImagesFolder);
    sourceDir = std::filesystem::canonical(sourceDir);

    if (!std::filesystem::exists(sourceDir)) {
        std::cerr << "Error: Base folder not found - " << sourceDir << "\n";
        return 1;
    }

    for (int i = 0; i < layers; ++i) {
        std::string sNumber = "_";
        if (i + 1 < 10) sNumber += "0";
        sNumber += std::to_string(i + 1);

        std::string internalPath = "/volume/layer" + sNumber + ".png";
        std::filesystem::path filePath = sourceDir / ("img" + sNumber + ".png");

        if (!std::filesystem::exists(filePath)) {
            std::cerr << "Error: File not found - " << filePath << "\n";
            return 1;
        }

        pImageStack->CreateSheetFromFile(i, internalPath, filePath.string());
    }

    // Set up a function from the image stack
    auto funcFromImageStack = model->AddFunctionFromImage3D(pImageStack.get());
    funcFromImageStack->SetDisplayName("Function from Image Stack");

    // Example of setting tile styles (can be customized)
    funcFromImageStack->SetTileStyles(
        Lib3MF::eTextureTileStyle::Wrap,
        Lib3MF::eTextureTileStyle::Clamp,
        Lib3MF::eTextureTileStyle::Mirror);

    auto writer = model->QueryWriter("3mf");
    writer->WriteToFile("ImageStack.3mf");

    std::cout << "Saved ImageStack.3mf\n";
    return 0;
}