#pragma once

#include "DataSerializer.h"

class Host {
    DataSerializer serializer;
    SocketInterface* socketInterface;
public:
    explicit Host(SocketInterface* _si);
    void exchange(std::variant<std::string, SimpleStruct> msg);
    void send(std::variant<std::string, SimpleStruct> msg);
    std::variant<std::string, SimpleStruct> receive();
};