#pragma once

#include "SocketInterface.h"


class SocketTCP: public SocketInterface{
    Socket sock;
public:
    SocketTCP(std::string ip = "127.0.0.1" , int port = 8000,  bool is_serv = false);

    void Send(std::vector<char> msg) override;
    std::vector<char> Receive(bool echo = false) override;
    std::vector<std::vector<char>> ReceiveAll() override;
    std::vector<char> ReceiveEcho(int which_one);
    void SendSpecific(std::vector<char> msg, int which_one);

};