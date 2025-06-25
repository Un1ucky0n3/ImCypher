#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <ranges>
#include "../Pixel/pixel.hpp"

auto ppmExtractMagicNumber(std::string const& path) -> bool;
auto ppmExtractFileHeader(std::string const& path) -> std::string;
auto ppmExtractFilePixels(std::string const& path) -> std::vector<Pixel>;
auto ppmExtractFilePixelsP6(std::string const& path) -> std::vector<Pixel>;
auto ppmExtractFilePixelsP3(std::string const& path) -> std::vector<Pixel>;
auto writePPM(const std::string& filename, const std::string& header, std::vector<Pixel> pixels) -> void;
auto ppmGetInfo(std::string const& path) -> void;