#pragma once

#include "SocketUDP.h"
#include "SocketTCP.h"
#include "Host.h"
#include <iostream>
#include <array>

class Gate{ 
    std::string ipAdress;
    int port;
    SocketInterface* sensorI;
    std::array<SocketInterface*, 4> serwerI;
public:
    Host* sensorGate;
    std::array<Host*, 4> serwerGate;
    Gate();
    ~Gate();
};