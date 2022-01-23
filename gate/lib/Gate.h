#pragma once

#include "SocketUDP.h"
#include "SocketTCP.h"
#include "Host.h"
#include "Header.h"
#include "Agregator.h"
#include "SHA256.h"
#include <CkCrypt2.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>

class Gate{ 
    std::string ipAdress;
    int port, nServers;
    SocketInterface* sensorI;
    std::vector<SocketInterface*> serwerI;
    Agregator agregator;

public:
    Host* sensorGate;
    std::vector<Host*> serwerGate;
    Gate(std::string ip = "127.0.0.1", int pt = 8000, int nsv=4);
    ~Gate();
    bool AgregateData(uint8_t which_server, uint16_t document_id, uint16_t part, uint16_t all_parts, std::vector<char> data, int timestamp);
    uint32_t GetHash(std::string data);
    void EraseAgregatedData(uint8_t which_server, uint16_t document_id);
    std::vector<char> ConstructDocumentMsg(uint8_t which_server, uint16_t document_id);

};