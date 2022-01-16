#pragma once

#include "SocketUDP.h"
#include "SocketTCP.h"
#include "Host.h"
#include <iostream>
#include <vector>

class Gate{ 
    std::string ipAdress;
    int port, nServers;
    SocketInterface* sensorI;
    std::vector<SocketInterface*> serwerI;
public:
    Host* sensorGate;
    std::array<Host*, 4> serwerGate;
    Gate(std::string ip = "127.0.0.1", int pt = 8000, int nsv=4);
    ~Gate();
};