#include "Gate.h"
#include <iostream>
#include "SHA256.h"


Gate::Gate(std::string ip, int pt, int nsv):agregator(nsv){
    ipAdress = ip;
    port = pt;
    nServers = nsv;
    std::map<uint16_t, std::vector<std::vector<char>>> buff;
    std::cout<<"Waiting for connection from servers: 0/" << std::to_string(nServers) << std::endl;
    for(int i =0; i<nServers; i++){
        serwerI.push_back(new SocketTCP(ipAdress, port+1+i,true));
        serwerGate.push_back(new Host(serwerI[i]));
        std::cout<<"Waiting for connection from servers: "<<std::to_string(i+1) << "/" << std::to_string(nServers) << std::endl;
    }
    sensorI = new SocketUDP(ipAdress, port,true);
    sensorGate = new Host(sensorI);
    std::cout<<"All servers connected, listening to sensors...\n";
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
        if(agregator.packetCounter[which_server][document_id]!=part){
            agregator.error[which_server][document_id]=1;
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
            agregator.error[which_server][document_id]=0;
        }
    if(agregator.packetCounter[which_server][document_id] == all_parts){
        return true;
    } 
    return false;
}

std::vector<char> Gate::ConstructDocumentMsg(uint8_t which_server, uint16_t document_id){
    std::vector<char> msg = agregator.docBuilder[which_server][document_id];
    std::string hash = Gate::GetHash(Utils::deserializeString(msg));
    AgregatedHeader ah = {htons(document_id), which_server, agregator.error[which_server][document_id], hash};
    msg = Utils::addHeader<AgregatedHeader>(ah, msg);
    EraseAgregatedData(which_server, document_id);
    return msg;
}

std::string Gate::GetHash(std::string data){
    SHA256 sha;
    sha.update(data);
    uint8_t * digest = sha.digest();
    return SHA256::toString(digest).substr(0, 32);
}

void Gate::EraseAgregatedData(uint8_t which_server, uint16_t document_id){
    agregator.docBuilder[which_server].erase(document_id);
    agregator.packetCounter[which_server].erase(document_id);
    agregator.timestamps[which_server].erase(document_id);
    agregator.error[which_server].erase(document_id);
}


