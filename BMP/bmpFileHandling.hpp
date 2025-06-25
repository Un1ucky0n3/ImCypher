#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "../Pixel/pixel.hpp"

auto bmpExtractMagicNumber(std::string const& path) -> bool;
auto bmpExtractType(std::string const& path) -> int;
auto bmpExtractFileHeader(std::string const& path) -> std::string;
auto bmpExtractFilePixels(std::string const& path) -> std::vector<Pixel>;
auto bmpExtractChannelAlpha(std::string const& path) -> std::vector<unsigned char>;
auto writeBMP(const std::string& filename, const std::string& header, std::vector<Pixel> pixels, std::vector<unsigned char> alpha) -> void;
auto bmpGetInfo(std::string const& path) -> void;