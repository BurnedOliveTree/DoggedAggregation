#pragma once

#include "SocketInterface.h"

class SocketUDP: public SocketInterface{
    Socket sock;
    std::map<uint8_t, sockaddr> client_history;

public:
    SocketUDP(std::string ip = "127.0.0.1" , int port = 8000,  bool is_serv = false);
    void Send(std::vector<char> msg) override;
    std::vector<char> Receive(bool echo= false) override;
    std::vector<char> ReceiveRaw(bool echo = false) override;
    void RememberClient(std::pair<std::vector<char>,sockaddr> data);
    sockaddr ChooseClient(std::vector<char> data);
};