#include "messageHandling.hpp"

#include <iostream>
#include <ostream>

auto decryptMessage(std::vector<Pixel> pixels, fileTypeEnum decryptionMode) -> std::string{
    unsigned char singleCharacter = 0b00000000;
    auto res = std::string("");
    auto iterator = pixels.begin();
    int i = 1;
    int color = 1;
    while (true) {
        if (iterator == pixels.end()) break;
        int colorToExtract = 0;
        if (color == 1) {
            if(decryptionMode == fileTypeEnum::BMP)
                colorToExtract = (int) (*iterator).b;
            else if (decryptionMode == fileTypeEnum::PPM)
                colorToExtract = (int) (*iterator).r;
        }
        if (color == 2)
            colorToExtract = (int) (*iterator).g;
        if (color == 3) {
            if(decryptionMode == fileTypeEnum::BMP)
                colorToExtract = (int) (*iterator).r;
            else if (decryptionMode == fileTypeEnum::PPM)
                colorToExtract = (int) (*iterator).b;
            color = 0;
            iterator++;
        }
        singleCharacter = (singleCharacter << 1) | (0b00000001 & colorToExtract);
        if (i % 8 == 0) {
            if(i==8 && (char)singleCharacter != '\"') return "\0";
            if((i!=8) && (char)singleCharacter == '\"') break;
            res += (char) singleCharacter;
            singleCharacter = 0;
        }
        i++;
        color++;
    }
    return res.substr(1, res.length() - 1);
}

auto encryptMessage(std::vector<Pixel> pixels, std::string message, fileTypeEnum encryptionMode) -> std::vector<Pixel> {
    int color = 1;
    auto iterator = pixels.begin();
    message = '\"' + message + '\"';
    for(unsigned char ch : message) {
        for(int i = 0; i < 8; i++) {
            int bit = (ch >> (7 - i)) & 0b00000001;
            if (color == 1) {
                if(encryptionMode == fileTypeEnum::BMP) {
                    (*iterator).b &= 0b11111110;
                    (*iterator).b |= bit;
                }
                else if (encryptionMode == fileTypeEnum::PPM) {
                    (*iterator).r &= 0b11111110;
                    (*iterator).r |= bit;
                }
            }
            else if (color == 2) {
                 (*iterator).g &= 0b11111110;
                 (*iterator).g |= bit;
            }
            else if (color == 3) {
                if(encryptionMode == fileTypeEnum::BMP) {
                    (*iterator).r &= 0b11111110;
                    (*iterator).r |= bit;
                }
                else if (encryptionMode == fileTypeEnum::PPM) {
                    (*iterator).b &= 0b11111110;
                    (*iterator).b |= bit;
                }
                color = 0;
                iterator++;
            }
            color++;
        }
    }
    return pixels;
}

auto canMessageBeEncrypted(std::vector<Pixel> pixels, std::string message) -> bool{
    return
          (message.size()+2)*8 < pixels.size()*3;

}