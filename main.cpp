#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include "messageHandling/messageHandling.hpp"
#include "fileCheck/fileCheck.hpp"
#include "BMP/bmpFileHandling.hpp"
#include "PPM/ppmFileHandling.hpp"
#include "Flags/flags.hpp"

enum class Flag {
    NONE,
    INFO,
    ENCRYPT,
    DECRYPT,
    CHECK,
    HELP
};

int main(int argc, char *argv[]) {
    int exitCode = 0;
    auto vectorArgs = std::vector<std::string>{"-i", "--info", "-e", "--encrypt", "-d", "--decrypt", "-c", "--check", "-h", "--help"};
    auto flag = Flag::NONE;
    auto argument = std::string("");
    auto message = std::string("");
    auto path = std::string("");
    switch(argc) {
        case 1:
            flag = Flag::HELP;
            break;
        case 2:
            argument = argv[1];
            if(argument == "-h" || argument == "--help")
                flag = Flag::HELP;
            else if(std::ranges::find(vectorArgs, argument) != vectorArgs.end()) {
                std::cerr << "Error: Invalid use of flag: " << argument << std::endl;
                exitCode = 5;
                break;
            }
            else {
                std::cerr << "Error: Not existing flag used: " << argument << std::endl;
                exitCode = 4;
                break;
            }
            break;
        case 3:
            argument = argv[1];
            if(argument == "-i" || argument == "--info")
                flag = Flag::INFO;
            else if(argument == "-d" || argument == "--decrypt")
                flag = Flag::DECRYPT;
            else if(std::ranges::find(vectorArgs, argument) != vectorArgs.end()) {
                std::cerr << "Error: Invalid use of flag: " << argument << std::endl;
                exitCode = 5;
                break;
            }
            else {
                std::cerr << "Error: Not existing flag used: " << argument << std::endl;
                exitCode = 4;
                break;
            }
            path = argv[2];
            break;
        case 4:
            argument = argv[1];
            if(argument == "-e" || argument == "--encrypt")
                flag = Flag::ENCRYPT;
            else if(argument == "-c" || argument == "--check")
                flag = Flag::CHECK;
            else if(std::ranges::find(vectorArgs, argument) != vectorArgs.end()) {
                std::cerr << "Error: Invalid use of flag: " << argument << std::endl;
                exitCode = 5;
                break;
            }
            else {
                std::cerr << "Error: Not existing flag used: " << argument << std::endl;
                exitCode = 4;
                break;
            }
            path = argv[2];
            message = argv[3];
            break;
        default:
            std::cerr << "Error: Too many arguments given!" << std::endl;
            exitCode = 6;
            break;
    }
    if(exitCode!=0) {
        std::cerr << "Use -h/--help to clarify." << std::endl;
        std::cerr <<"\nProcess failed with exit code "<< exitCode << std::endl;
        return exitCode;
    }

    //============================================================================

    switch(flag) {
        case Flag::INFO:
            exitCode = info(path);
            break;
        case Flag::HELP:
            help();
            break;
        case Flag::ENCRYPT:
            if(std::ranges::find(message, '\"') != message.end()) {
                std::cerr << "Error: Message is in wrong format: " << message << std::endl;
                exitCode = 7;
            }
            else
                exitCode = encrypt(path, message);
            break;
        case Flag::DECRYPT:
            exitCode = decrypt(path);
            break;
        case Flag::CHECK:
            if(std::ranges::find(message, '\"') != message.end()) {
                std::cerr << "Error: Message is in wrong format: " << message << std::endl;
                exitCode = 7;
            }
            else
                exitCode = check(path, message);
            break;
        case Flag::NONE:
            std::cerr << "Error: Unexpected error occurred" << std::endl;
            exitCode = 1;
            break;
    }

    //============================================================================
    if(exitCode == 0) {
        std::cout <<"\nProcess finished with exit code "<< exitCode << std::endl;
    }
    else {
        std::cerr << "Use -h/--help to clarify." << std::endl;
        std::cerr <<"\nProcess failed with exit code "<< exitCode << std::endl;
    }
    return exitCode;
}