#pragma once
#include "Socket.h"
#include "Utils.h"

struct PacketHeader{
    uint16_t timestamp;
    uint16_t total;
    uint16_t current;
};

struct StandardHeader {
    uint16_t documentId;
    uint8_t documentType;
    uint8_t type;
};

class SocketInterface{
public:
    virtual ~SocketInterface() = default;
    virtual void exchange(StandardHeader, std::vector<char>) = 0;
    virtual void send(std::vector<char> message) = 0;
    virtual std::vector<char> receive(int flag = 0) = 0;
};