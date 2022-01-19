#pragma once
#include "Socket.h"
#include "Utils.h"

struct PacketHeader{
    uint16_t timestamp;
    uint16_t total;
    uint16_t current;
};

class SocketInterface{
public:
    virtual ~SocketInterface() = default;
    virtual void exchange(std::vector<char> message) = 0;
    virtual void send(std::vector<char> message) = 0;
    virtual std::vector<char> receive(int flag = 0) = 0;
};