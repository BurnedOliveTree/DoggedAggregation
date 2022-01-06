#pragma once

#include "DataSerializer.h"

class Host {
    DataSerializer serializer;
public:
    explicit Host(SocketInterface* _si);
    void send(std::variant<std::string, SimpleStruct> msg);
    std::variant<std::string, SimpleStruct> receive();
};