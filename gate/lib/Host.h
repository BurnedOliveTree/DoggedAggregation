#pragma once

#include "DataSerializer.h"

class Host{
    DataSerializer ws;
public:
    Host(SocketInterface* _si);
    void Send(std::variant<std::string, SimpleStruct> msg);
    void Send(std::vector<char> msg);
    std::variant<std::string, SimpleStruct> Receive(bool echo = false);
    std::vector<char> ReceiveRaw(bool echo = false);
};