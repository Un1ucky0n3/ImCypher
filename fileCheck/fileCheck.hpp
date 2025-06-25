#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "fileTypeEnum.hpp"

auto fileExists(std::string const& path) -> bool;
auto fileType(std::string const& path) -> fileTypeEnum;
