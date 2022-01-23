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
    virtual void send(StandardHeader, std::vector<char>) = 0;
    virtual std::vector<char> receive(uint8_t flag) = 0;
};