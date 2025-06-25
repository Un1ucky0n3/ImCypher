cmake_minimum_required(VERSION 3.20)
project(ImCypher)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImCypher
        BMP/bmpFileHandling.cpp
        fileCheck/fileCheck.cpp
        main.cpp
        messageHandling/messageHandling.cpp
        PPM/ppmFileHandling.cpp
        Flags/flags.cpp
)
