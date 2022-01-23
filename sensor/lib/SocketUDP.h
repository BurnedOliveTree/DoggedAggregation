#pragma once

#include "SocketInterface.h"
#include "Timer.h"
#include "DataSerializer.h"

class SocketUDP: public SocketInterface{
    Socket sock;
    Timer* timer;
    uint16_t timeoutAfter;

    bool isACK(PacketHeader, StandardHeader, std::vector<char>);
public:
    explicit SocketUDP(const std::string& ipAddress, int port);
    void send(StandardHeader, std::vector<char>) override;
    std::vector<char> receive(uint8_t) override;
};