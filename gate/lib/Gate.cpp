#include "Gate.h"
#include <iostream>

Gate::Gate(std::string ip, int pt, int nsv):agregator(nsv){
    ipAdress = ip;
    port = pt;
    nServers = nsv;
    sensorI = new SocketUDP(ipAdress, port,true);
    sensorGate = new Host(sensorI);
    std::map<uint16_t, std::vector<std::vector<char>>> buff;
    for(int i =0; i<4; i++){
        serwerI.push_back(new SocketTCP(ipAdress, port+1+i,true));
        serwerGate.push_back(new Host(serwerI[i]));
    }
}

Gate::~Gate(){
    delete sensorI;
    delete sensorGate;
    for(int i =0; i<4; i++){
        delete serwerI[i];
        delete serwerGate[i];
    }
}

bool Gate::AgregateData(uint8_t which_server, uint16_t document_id, std::vector<char> data)
{

}

