#ifndef DECODER_H
#define DECODER_H

#include <string>

#include "Message.h"


class Decoder
{
private:
    const std::string charsBase64{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

    static inline bool isBase64(const unsigned char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }


public:
    std::string decodeBase64(const std::string& encodedString);
    Message decodeMessage(const std::string& encodedString);
    std::vector<unsigned char> decodeImage(const std::string& encodedString);
};

#endif