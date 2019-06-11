#include "Decoder.h"


std::string Decoder::decodeBase64(const std::string& encodedString)
{
    int in_len = encodedString.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encodedString[in_] != '=') && isBase64(encodedString[in_]))
    {
        char_array_4[i++] = encodedString[in_]; in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                char_array_4[i] = charsBase64.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
            {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
        {
            char_array_4[j] = charsBase64.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
        {
            ret += char_array_3[j];
        }
    }

    return ret;
}

Message Decoder::decodeMessage(const std::string& encodedString)
{
    msgpack::object_handle oh = msgpack::unpack(encodedString.data(), encodedString.size());
    Message message = oh.get().as<Message>();
    return message;
}

std::vector<unsigned char> Decoder::decodeImage(const std::string& encodedString)
{
    return std::vector<unsigned char>(encodedString.begin(), encodedString.end());
}