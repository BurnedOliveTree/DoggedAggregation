#pragma once

#include "SocketInterface.h"

class SocketUDP: public SocketInterface{
    Socket sock;
public:
    explicit SocketUDP(const std::string& ipAddress = "127.0.0.1" , int port = 8000);
    void send(std::vector<char> msg) override;
    std::vector<char> receive() override;
};