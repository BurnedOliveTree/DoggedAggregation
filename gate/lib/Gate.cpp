#include "Gate.h"
#include <iostream>

Gate::Gate(std::string ip, int pt, int nsv):agregator(nsv){
    ipAdress = ip;
    port = pt;
    nServers = nsv;
    std::map<uint16_t, std::vector<std::vector<char>>> buff;
    std::cout<<"Waiting for connection from servers: 0/" << std::to_string(nServers) << std::endl;
    for(int i =0; i<nServers; i++){
        // serwerI.push_back(new SocketTCP(ipAdress, port+1+i,true));
        // serwerGate.push_back(new Host(serwerI[i]));
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
    RememberType(which_server);
    if(agregator.docBuilder[which_server].count(document_id)){
        //dokument już tu jest
        if(agregator.packetCounter[which_server][document_id]>part){
            // i odebraliśmy już ten pakiet
            return false;
        }
        if(agregator.packetCounter[which_server][document_id]!=part){
            // jeśli ne oczekiwaliśmy tego partu, ustawiamy uszkodzony dokument i odsyłamy
            agregator.error[which_server][document_id]=1;
            return true;
        }
        else{
            // Agregujemy part do całości dokumentu
            agregator.docBuilder[which_server][document_id].insert( agregator.docBuilder[which_server][document_id].end(), data.begin(), data.end());
            agregator.packetCounter[which_server][document_id]++;
        }
    }
    else if(part == 0)
    {
        // Jeśli to pierwszy part, dodaj nowy dokument do agregacji
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
    AgregatedHeader ah = {htons(document_id), which_server, agregator.error[which_server][document_id], htonl(0)};   //Tutaj za to zero musi być hash
    msg = Utils::addHeader<AgregatedHeader>(ah,msg);
    EraseAgregatedData(which_server, document_id);
    return msg;
}

void Gate::EraseAgregatedData(uint8_t which_server, uint16_t document_id){
    agregator.docBuilder[which_server].erase(document_id);
    agregator.packetCounter[which_server].erase(document_id);
    agregator.timestamps[which_server].erase(document_id);
    agregator.error[which_server].erase(document_id);
}

int Gate::RememberType(uint8_t doc_type){
    for (int i = 0; i < knownTypes.size(); i++){
        if(knownTypes[i] == doc_type){
            return 0;
        }
    }
    knownTypes.push_back(doc_type);
    return 1;
}


void Gate::SynchronizeTime(){
    auto timer = &Timer::getInstance();
    bool run = true;
    while(run){
        auto curr_time = timer->getCounter() % 10;
        std::cout << "Synchronizing... " << curr_time << "\n";
        for (int i = 0; i < knownTypes.size(); i++){
            std::cout << "Synchronizing time for: " << unsigned(knownTypes[i]) << std::endl; 
            std::vector<char> msg = Utils::serializeStruct<uint16_t>(curr_time);
            DocumentHeader dh = {0, knownTypes[i], 1};
            PHeader ph = {htons(curr_time), 1, 0};
            auto sdh = Utils::serializeStruct<DocumentHeader>(dh);
            msg = Utils::addHeader(sdh, msg);
            auto sph = Utils::serializeStruct<PHeader>(ph);
            msg = Utils::addHeader(sph, msg);
            Utils::printVector(msg);
            sensorGate->Send(msg);
        }
        run = false;
    }
}

