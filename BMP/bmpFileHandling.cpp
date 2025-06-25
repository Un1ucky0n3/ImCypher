#include "bmpFileHandling.hpp"

auto bmpExtractMagicNumber(std::string const& path) -> bool {
    auto file = std::ifstream(path, std::ios::binary);
    auto first = file.get();
    auto second = file.get();
    return
        (first == 'B' && second == 'M') ||
        (first == 'B' && second == 'A') ||
        (first == 'C' && second == 'I') ||
        (first == 'C' && second == 'P') ||
        (first == 'I' && second == 'C') ||
        (first == 'P' && second == 'T');
}
auto bmpExtractType(std::string const& path) -> int {
    auto file = std::ifstream(path,std::ios::binary);
    file.seekg(14, std::ios::beg);
    auto type = ((unsigned char)file.get()) |
                    ((unsigned char)file.get()) << 8 |
                    ((unsigned char)file.get()) << 16 |
                    ((unsigned char)file.get()) << 24;
    if(type!=40 && type!=108 && type!=124)
        return 0;
    return type;
}
auto bmpExtractFileHeader(std::string const& path) -> std::string{
    auto file = std::ifstream(path,std::ios::binary);
    auto res = std::string("");
    auto headerlength = bmpExtractType(path)+14;
    for(int i = 0; i<headerlength; i++) {
        res += file.get();
    }
    return res;
}
auto bmpExtractFilePixels(std::string const& path) -> std::vector<Pixel>{
    auto file = std::ifstream(path,std::ios::binary);
    auto headerLength = bmpExtractType(path)+14;
    file.seekg(headerLength, std::ios::beg);
    auto pixels = std::vector<Pixel>();
    if(headerLength==54) {
        while(!file.eof()) {
            unsigned char b, g, r;
            b = file.get();
            g = file.get();
            r = file.get();
            pixels.push_back({r, g, b});
        }
    }
    else if (headerLength==122 || headerLength==138) {
        while(!file.eof()) {
            unsigned char b, g, r;
            b = file.get();
            g = file.get();
            r = file.get();
            file.get();
            pixels.push_back({r, g, b});
        }
    }
    return pixels;
}
auto bmpExtractChannelAlpha(std::string const& path) -> std::vector<unsigned char> {
    auto headerLength = bmpExtractType(path)+14;
    if (headerLength == 54) return std::vector<unsigned char>();
    auto file = std::ifstream(path, std::ios::binary);
    auto alpha = std::vector<unsigned char>();
    file.seekg(headerLength, std::ios::beg);
    while(!file.eof()) {
        file.get();
        file.get();
        file.get();
        alpha.push_back(file.get());
    }
    return alpha;
}
auto writeBMP(const std::string& filename, const std::string& header, std::vector<Pixel> pixels, std::vector<unsigned char> alpha) -> void{
    auto file = std::ofstream(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }
    for(char ch : header) {
        file.put(ch);
    }
    if(alpha.empty()) {
        for(auto pixel : pixels) {
            file.put(pixel.b);
            file.put(pixel.g);
            file.put(pixel.r);
        }
    }
    else {
        auto alphaIterator = alpha.begin();
        for(auto pixel : pixels) {
            file.put(pixel.b);
            file.put(pixel.g);
            file.put(pixel.r);
            file.put(*alphaIterator);
            alphaIterator++;
        }
    }
}
auto bmpGetInfo(std::string const& path) -> void{
    auto file = std::ifstream(path,std::ios::binary);
    auto fileSize = std::filesystem::file_size(path);
    file.seekg(18, std::ios::beg);
    auto width = file.get() | (file.get() << 8) | (file.get() << 16) | (file.get() << 24);
    auto height = file.get() | (file.get() << 8) | (file.get() << 16) | (file.get() << 24);
    auto const lastModificationTime = std::filesystem::last_write_time(path);
    std::cout << "File: " << path << " is in .bmp format:";
    if(bmpExtractType(path)==40)
        std::cout << " V3 24-bit uncompressed" << std::endl;
    else if(bmpExtractType(path)==108)
        std::cout << " V4 32-bit uncompressed" << std::endl;
    else if (bmpExtractType(path) == 124)
        std::cout << " V5 32-bit uncompressed" << std::endl;
    std::cout << "File size: " << fileSize << " bytes." << std::endl;
    std::cout << "Image size: " << width << " " << height << std::endl;
    std::cout << std::format("File write time is {}", lastModificationTime) << std::endl;
}