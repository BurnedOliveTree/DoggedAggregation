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

bool Gate::AgregateData(uint8_t which_server, uint16_t document_id, uint16_t part, uint16_t all_parts, std::vector<char> data, int timestamp)
{
    if(agregator.docBuilder[which_server].count(document_id)){
        //dokument już tu jest
        if(agregator.packetCounter[which_server][document_id]<=part){
            return false;
        }
        else{
            agregator.docBuilder[which_server][document_id].insert( agregator.docBuilder[which_server][document_id].end(), data.begin(), data.end());
            agregator.packetCounter[which_server][document_id]++;
        }
    }
    else{
            agregator.docBuilder[which_server][document_id] = data;
            agregator.packetCounter[which_server][document_id] = 1;
            agregator.timestamps[which_server][document_id] = timestamp;
        }
    if(agregator.packetCounter[which_server][document_id] = all_parts){
        return true;
    } 
}

