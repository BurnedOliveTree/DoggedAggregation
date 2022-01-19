#pragma once
#include "Socket.h"
#include "Utils.h"

struct PacketHeaderTCP{
    uint16_t length;
    uint8_t max_packet;
    uint8_t curr_packet;
};

struct PacketHeaderUDP{
    uint16_t timestamp;
    uint32_t total;
    uint32_t current;
};

class SocketInterface{
public:
    virtual void Send(std::vector<char> msg) = 0;
    virtual std::vector<char> Receive(bool echo = false) = 0;
    virtual std::vector<char> ReceiveRaw(bool echo = false) = 0;
};