#include "../fileCheck/fileTypeEnum.hpp"
#include "../PPM/ppmFileHandling.hpp"
#include "../messageHandling/messageHandling.hpp"
#include "../BMP/bmpFileHandling.hpp"
#include "../fileCheck/fileCheck.hpp"
#include <iostream>
#include "../Pixel/pixel.hpp"
#include <string>

auto info(std::string const& path) -> int;
auto encrypt(std::string const& path, std::string const& message) -> int;
auto decrypt(std::string const& path) -> int;
auto check(std::string const& path, std::string const& message) -> int;
auto help() -> void;
