#pragma once

#include "DataSerializer.h"

class Host {
    SocketInterface* socketInterface;
public:
    explicit Host(SocketInterface* _si);
    void send(std::variant<std::string, uint16_t>, uint16_t, uint8_t);
    std::variant<std::string, uint16_t> receive(uint8_t);
};