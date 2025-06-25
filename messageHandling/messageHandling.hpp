#include <vector>
#include <string>
#include "../Pixel/pixel.hpp"
#include "../fileCheck/fileTypeEnum.hpp"

auto decryptMessage(std::vector<Pixel> pixels, fileTypeEnum decryptionMode) -> std::string;
auto encryptMessage(std::vector<Pixel> pixels, std::string message, fileTypeEnum decryptionMode) -> std::vector<Pixel>;
auto canMessageBeEncrypted(std::vector<Pixel> pixels, std::string message) -> bool;