#include "fileCheck.hpp"

auto fileExists(std::string const& path) -> bool{
    auto file = std::fstream(path);
    if(not file){
        std::cerr << "Error: Couldn't open file, path leads to nowhere." << std::endl;
        file.close();
        return false;
    }
    return true;
}
auto fileType(std::string const& path) -> fileTypeEnum{
    auto file = std::filesystem::path(path);
    auto fileExtension = file.extension();
    auto res = fileTypeEnum();
    if(fileExtension == ".bmp")
        res = fileTypeEnum::BMP;
    else if(fileExtension == ".ppm")
        res = fileTypeEnum::PPM;
    else
        res = fileTypeEnum::ERROR;
    return res;
}
