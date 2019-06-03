#ifndef RECEIVER_H
#define RECEIVER_H

#include <string>

#include <zmq.hpp>


class Receiver
{
private:
    zmq::context_t context;
    zmq::socket_t subscriber;
    std::string url;
    std::string topic;

public:
    Receiver(const std::string broker, const std::string port, const std::string topic);
    void connect();
    std::string receive();
    void disconnect();
};

#endif