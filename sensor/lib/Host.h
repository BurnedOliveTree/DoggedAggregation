#pragma once

#include "DataSerializer.h"

class Host {
    SocketInterface* socketInterface;
public:
    explicit Host(SocketInterface* _si);
    void exchange(std::variant<std::string, uint16_t> msg, uint16_t documentId, uint8_t documentType);
    void send(std::variant<std::string, uint16_t> msg, uint16_t documentId, uint8_t documentType);
    std::variant<std::string, uint16_t> receive();
    uint16_t receiveTime();
};