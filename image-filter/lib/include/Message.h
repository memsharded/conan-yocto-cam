#ifndef MESSAGE_H
#define MESSAGE_H


#include <string>

#include <msgpack.hpp>


struct Message
{
    std::map<std::string, double> user;
    std::map<std::string, double> pilot;
    std::string mode;
    std::string img;
    MSGPACK_DEFINE_MAP(user, pilot, mode, img);
};

#endif