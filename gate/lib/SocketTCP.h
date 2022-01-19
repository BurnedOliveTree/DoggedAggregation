#pragma once

#include "SocketInterface.h"
#include "Header.h"


class SocketTCP: public SocketInterface{
    Socket sock;
public:
    SocketTCP(std::string ip = "127.0.0.1" , int port = 8000,  bool is_serv = false);
    void Send(std::vector<char> msg) override;
    std::vector<char> Receive(bool echo = false) override;
    std::vector<char> ReceiveRaw(bool echo = false) override;
};