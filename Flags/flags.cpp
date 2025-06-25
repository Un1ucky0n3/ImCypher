#include "flags.hpp"

auto info(std::string const& path) -> int{
    if(!fileExists(path)){
        return 2;
    }
    switch (fileType(path)){
        case fileTypeEnum::BMP:
            if(!bmpExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            if(bmpExtractType(path) == 0) {
                std::cerr << "Error: BMP format type not supported" << std::endl;
                return 8;
            }
            bmpGetInfo(path);
            return 0;
        case fileTypeEnum::PPM:
            if(!ppmExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            ppmGetInfo(path);
            return 0;
        case fileTypeEnum::ERROR:
            std::cerr << "Error: given file's type is not supported by this program." << std::endl;
            return 3;
        default:
            std::cerr << "Error: unknown error occurred." << std::endl;
            return 1;
    }
}
auto encrypt(std::string const& path, std::string const& message) -> int{
    if(!fileExists(path)){
        return 2;
    }
    auto pixels = std::vector<Pixel>();
    auto header = std::string();
    auto alpha = std::vector<unsigned char>();
    switch (fileType(path)){
        case fileTypeEnum::BMP:
            if(!bmpExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            if(bmpExtractType(path) == 0) {
                std::cerr << "Error: BMP format type not supported" << std::endl;
                return 8;
            }
            pixels = bmpExtractFilePixels(path);
            if(!canMessageBeEncrypted(pixels, message)) {
                std::cerr << "Error: message is too long." << std::endl;
                return 7;
            }
            pixels = encryptMessage(pixels, message, fileTypeEnum::BMP);
            header = bmpExtractFileHeader(path);
            alpha = bmpExtractChannelAlpha(path);
            writeBMP(path, header, pixels, alpha);
            return 0;
        case fileTypeEnum::PPM:
            if(!ppmExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            pixels = ppmExtractFilePixels(path);
            if(!canMessageBeEncrypted(pixels, message)) {
                std::cerr << "Error: message is too long." << std::endl;
                return 7;
            }
            pixels = encryptMessage(pixels, message, fileTypeEnum::PPM);
            header = ppmExtractFileHeader(path);
            writePPM(path, header, pixels);
            return 0;
        case fileTypeEnum::ERROR:
            std::cerr << "Error: given file's type is not supported by this program." << std::endl;
            return 3;
        default:
            std::cerr << "Error: unknown error occurred." << std::endl;
            return 1;
    }
}
auto decrypt(std::string const& path) -> int {
    if(!fileExists(path)){
        return 2;
    }
    auto pixels = std::vector<Pixel>();
    auto message = std::string();
    switch (fileType(path)){
        case fileTypeEnum::BMP:
            if(!bmpExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            if(bmpExtractType(path) == 0) {
                std::cerr << "Error: BMP format type not supported" << std::endl;
                return 8;
            }
            pixels = bmpExtractFilePixels(path);
            message = decryptMessage(pixels, fileTypeEnum::BMP);
            if(message == "\0")
                std::cout << "There is no message written." << std::endl;
            else
                std::cout << "Decrypting message from file: " << path << "\n"
                            << "Message: " << message << std::endl;
            return 0;
        case fileTypeEnum::PPM:
            if(!ppmExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            pixels = ppmExtractFilePixels(path);
            message = decryptMessage(pixels, fileTypeEnum::PPM);
            if(message == "\0")
                std::cout << "There is no message written." << std::endl;
            else
                std::cout << "Decrypting message from file: " << path << "\n"
                            << "Message: " << message << std::endl;
            return 0;
        case fileTypeEnum::ERROR:
            std::cerr << "Error: given file's type is not supported by this program." << std::endl;
            return 3;
        default:
            std::cerr << "Error: unknown error occurred." << std::endl;
            return 1;
    }
}
auto check(std::string const& path, std::string const& message) -> int {
    if(!fileExists(path)) {
        return 2;
    }
    auto pixels = std::vector<Pixel>();
    switch (fileType(path)){
        case fileTypeEnum::BMP:
            if(!bmpExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            if(bmpExtractType(path) == 0) {
                std::cerr << "Error: BMP format type not supported" << std::endl;
                return 8;
            }
            pixels = bmpExtractFilePixels(path);
            break;
        case fileTypeEnum::PPM:
            if(!ppmExtractMagicNumber(path)) {
                std::cerr << "Error: File is corrupted" << std::endl;
                return 9;
            }
            pixels = ppmExtractFilePixels(path);
            break;
        case fileTypeEnum::ERROR:
            std::cerr << "Error: given file's type is not supported by this program." << std::endl;
            return 3;
        default:
            std::cerr << "Error: unknown error occurred." << std::endl;
            return 1;
    }
    if(!canMessageBeEncrypted(pixels, message)) {
        std::cout << "Message: \"" << message << "\" is too long for file: " << path << std::endl;
        return 0;
    }
    else {
        std::cout << "Message: \"" << message << "\" is valid for file: "<< path << std::endl;
        return 0;
    }

}
auto help() -> void{
    std::cout
            << "ImCypher version 1.0 alpha for Windows 10\n"
            << "==================================\n\n"
            << "Description: Encrypt/Decrypt hidden messages in image files.\n\n"
            << "Supported file extensions:\n"
            << "\tBMP: V3 24-bit uncompressed / V4 32-bit uncompressed / V5 32-bit uncompressed\n"
            << "\tPPM: P3 ascii stream / P6 binary stream\n"
            << "\n"
            << "Usage:\n"
            << "  ImCypher.exe <flag> [file directory] [\"message\"]\n"
            << "Flags:\n"
            << "  -i , --info\t\tShow information about given file: capacity, dimensions, last modification date and timestamp.\n"
            << "  \t\t\tUsage: ImCypher.exe -i [file directory].\n\n"
            << "  -e , --encrypt\tEncrypt message in a file, both given by a user.\n"
            << "  \t\t\tUsage: ImCypher.exe -e [file directory] [\"message\"].\n\n"
            << "  -d , --decrypt\tDecrypt hidden message and show output from a file given by a user.\n"
            << "  \t\t\tUsage: ImCypher.exe -d [file directory].\n\n"
            << "  -c , --check\t\tCheck if message can be encrypted into a file, both given by a user.\n"
            << "  \t\t\tUsage: ImCypher.exe -c [file directory] [\"message\"].\n\n"
            << "  -h , --help\t\tDisplay information about the program.\n"
            << "  \t\t\tUsage: ImCypher.exe -h.\n"
            << "\n"
            << "Examples:\n"
            << "  ImCypher.exe -i C:\\users\\user\\images\\image.ppm\n"
            << "  ImCypher.exe -e C:\\users\\user\\images\\image.bmp \"My secret message!\"\n"
            << "  ImCypher.exe -d C:\\users\\user\\images\\image.bmp\n"
            << "  ImCypher.exe -c C:\\users\\user\\images\\image.ppm \"My test message to see if it fits\"\n"
            << "  ImCypher.exe -h\n"
            << "\n"
            << "Notes:\n"
            << "  Program modifies the original file, it does not create a copy.\n"
            << "  Always write your message between \" \" characters, do not include \" character as part of your message.\n"
            << "  Use only supported file types, any other type will result in an error.\n"
            << "\n"
            << "Exit Codes:\n"
            << "  0 Success\n"
            << "  1 General Error\n"
            << "  2 File path does not exist\n"
            << "  3 File type is not supported\n"
            << "  4 Not existing flag given\n"
            << "  5 Invalid use of a flag\n"
            << "  6 Too many arguments given\n"
            << "  7 Message is in wrong format or is too long\n"
            << "  8 Format type is not supported\n"
            << "  9 File does not contain the magic number (identificator)"
    << std::endl;
}