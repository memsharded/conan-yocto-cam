#include <iostream>
#include <string>

#include <fmt/format.h>

#include "Receiver.h"


Receiver::Receiver(const std::string broker, const std::string port, const std::string topic)
    : context{1},
    subscriber{context, ZMQ_SUB},
    url{fmt::format("tcp://{}:{}", broker, port)},
    topic{topic}
{
}

void Receiver::connect()
{
    subscriber.connect(url);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    std::cout << "Receiver::connect: " << url << std::endl;
}

std::string Receiver::receive()
{
    std::cout << "Receiver::receive(): Init" << std::endl;
    zmq::message_t env;
    subscriber.recv(&env);
    std::string env_str = std::string(static_cast<char*>(env.data()), env.size());
    std::cout << "Receiver::receive(): Received envelope '" << env_str << "'" << std::endl;

    zmq::message_t msg;
    subscriber.recv(&msg);
    std::string msg_str = std::string(static_cast<char*>(msg.data()), msg.size());
    std::cout << "Receiver::receive(): Received message!" << std::endl;
    return msg_str;
}

void Receiver::disconnect()
{
    subscriber.close();
    context.close();
}