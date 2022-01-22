#pragma once

#include "SocketInterface.h"
#include "Timer.h"
#include "DataSerializer.h"

class SocketUDP: public SocketInterface{
    Socket sock;
    uint16_t timeoutAfter;

    bool isACK(PacketHeader, PacketHeader, StandardHeader, StandardHeader);
public:
    explicit SocketUDP(const std::string& ipAddress, int port);
    void exchange(StandardHeader, std::vector<char>) override;
    void send(std::vector<char> message) override;
    std::vector<char> receive(int flag) override;
};