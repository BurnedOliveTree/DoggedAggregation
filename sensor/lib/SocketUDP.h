#pragma once

#include "SocketInterface.h"
#include "Timer.h"

class SocketUDP: public SocketInterface{
    Socket sock;
    uint16_t timeoutAfter;
public:
    explicit SocketUDP(const std::string& ipAddress, int port);
    void exchange(std::vector<char> message) override;
    void send(std::vector<char> message) override;
    std::vector<char> receive(int flag) override;
};