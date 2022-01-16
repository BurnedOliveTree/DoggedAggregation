#pragma once

#include "SocketUDP.h"
#include "SocketTCP.h"
#include "Host.h"
#include <iostream>
#include <vector>

struct StandardHeader {
    uint16_t documentId;
    uint8_t documentType;
    uint8_t type;
};

class Gate{ 
    std::string ipAdress;
    int port, nServers;
    SocketInterface* sensorI;
    std::vector<SocketInterface*> serwerI;
public:
    Host* sensorGate;
    std::vector<Host*> serwerGate;
    Gate(std::string ip = "127.0.0.1", int pt = 8000, int nsv=4);
    ~Gate();
};