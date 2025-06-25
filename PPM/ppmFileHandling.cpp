#include "ppmFileHandling.hpp"

auto ppmExtractMagicNumber(std::string const& path) -> bool {
    auto file = std::ifstream(path);
    auto magicNumber = std::string("");
    std::getline(file,  magicNumber);
    return (magicNumber == "P6" || magicNumber == "P3");
}
auto ppmExtractFileHeader(std::string const& path) -> std::string{
    auto file = std::fstream(path);
    auto res = std::string("");
    auto line = std::string("");
    std::getline(file, line);
    res += line + "\n";
    while(std::getline(file, line) && line[0] == '#') {
        res += line + "\n";
    }
    res += line + "\n";
    std::getline(file, line);
    res += line + "\n";
    return res;
}
auto ppmExtractFilePixels(std::string const& path) -> std::vector<Pixel> {
    auto file = std::fstream(path);
    auto line = std::string("");
    std::getline(file, line);
    if(line=="P6")
        return ppmExtractFilePixelsP6(path);
    else
        return ppmExtractFilePixelsP3(path);
};
auto ppmExtractFilePixelsP6(std::string const& path) -> std::vector<Pixel>{
    auto line = std::string("");
    auto file = std::ifstream(path, std::ios::binary);
    auto header = ppmExtractFileHeader(path);
    file.seekg(0, std::ios::beg);
    for(int i = 0; i < header.size(); i++)
        file.get();
    auto pixels = std::vector<Pixel>();
    while(!file.eof()) {
        unsigned char r, g, b;
        r = file.get();
        g = file.get();
        b = file.get();
        pixels.push_back({r, g, b});
    }
    return pixels;
}
auto ppmExtractFilePixelsP3(std::string const& path) -> std::vector<Pixel>{
    auto line = std::string("");
    auto file = std::ifstream(path);
    auto header = ppmExtractFileHeader(path);
    for(int i = 0; i < header.size(); i++)
        file.get();
    auto pixels = std::vector<Pixel>();
    int r = 0, g = 0, b = 0;
    while(!file.eof()) {
        file>>r>>g>>b;
        pixels.push_back({(unsigned char)r, (unsigned char)g, (unsigned char)b});
    }
    return pixels;
}
auto writePPM(const std::string& filename, const std::string& header, std::vector<Pixel> pixels) -> void{
    auto magicLine = header.substr(0,2);
    auto file = std::ofstream();
    if(magicLine=="P6")
        file = std::ofstream(filename, std::ios::binary);
    else
        file = std::ofstream(filename);
    if (!file) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }
    for(char ch : header) {
        file.put(ch);
    }
    if(magicLine=="P6")
        for(auto pixel : pixels) {
            file.put(pixel.r);
            file.put(pixel.g);
            file.put(pixel.b);
        }
    else {
        for(auto pixel : pixels) {
            file<<(int)pixel.r<<" "<<(int)pixel.g<<" "<<(int)pixel.b<<" ";
        }
    }
}
auto ppmGetInfo(std::string const& path) -> void{
    auto file = std::ifstream(path);
    auto header = ppmExtractFileHeader(path);
    auto fileSize = std::filesystem::file_size(path);
    auto line = std::string("");
    auto sizeOffset = std::ranges::count_if(header, [](char c){return c == '\n'; });
    std::getline(file, line);
    auto const lastModificationTime = std::filesystem::last_write_time(path);
    std::cout << "File: " << path << " is in ppm format: ";
    file.seekg(0, std::ios::beg);
    std::getline(file, line);
    std::cout << line << (line=="P6"?" (binary stream)":" (ascii stream)") <<"." <<std::endl;
    std::cout << "File size: " << fileSize << " bytes." << std::endl;
    file.seekg(0, std::ios::beg);
    for(int i = 0; i < sizeOffset-1; i++)
        std::getline(file, line);
    std::cout << "Image size: " << line << std::endl;
    std::cout << std::format("File write time is {}", lastModificationTime) << std::endl;
}