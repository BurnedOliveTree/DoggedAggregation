#include "Gate.h"
#include <iostream>

Gate::Gate(std::string ip, int pt, int nsv):agregator(nsv){
    ipAdress = ip;
    port = pt;
    nServers = nsv;
    std::map<uint16_t, std::vector<std::vector<char>>> buff;
    std::cout<<"Waiting for connection from servers: 0/" << std::to_string(nServers) << std::endl;
    sensorI = new SocketUDP(ipAdress, port,true);
    sensorGate = new Host(sensorI);
    for(int i =0; i<nServers; i++){
        serwerI.push_back(new SocketTCP(ipAdress, port+1+i,true));
        serwerGate.push_back(new Host(serwerI[i]));
        std::cout<<"Waiting for connection from servers: "<<std::to_string(i+1) << "/" << std::to_string(nServers) << std::endl;
    }
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
    uint32_t hash = Gate::GetHash(Utils::deserializeString(msg));
    AgregatedHeader ah = {htons(document_id), which_server, agregator.error[which_server][document_id], htonl(hash)};
    msg = Utils::addHeader<AgregatedHeader>(ah,msg);
    EraseAgregatedData(which_server, document_id);
    return msg;
}

uint32_t Gate::GetHash(std::string data){
    SHA256 sha;
    sha.update(data);
    uint8_t* digest = sha.digest();
    AES aes(AESKeyLength::AES_128);
    std::vector<char> short_hash;
    unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    unsigned int len = 0;
    uint8_t *out = aes.EncryptECB(digest, (16 * sizeof(unsigned char) * 2) * sizeof(unsigned char), key, len);
    for (int i = 0; i < 4; i++){
        short_hash.push_back(out[i]);
    }
    uint32_t* st = reinterpret_cast<uint32_t*>(short_hash.data());
    return *st;
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
    auto curr_time = timer->getCounter();
    std::cout << "\nSynchronizing... " << curr_time << "\n";
    for (int i = 0; i < knownTypes.size(); i++){
        std::vector<char> msg = Utils::serializeStruct<uint16_t>(curr_time);
        DocumentHeader dh = {0, knownTypes[i], 1};
        PHeader ph = {htons(curr_time), 1, 0};
        auto sdh = Utils::serializeStruct<DocumentHeader>(dh);
        msg = Utils::addHeader(sdh, msg);
        auto sph = Utils::serializeStruct<PHeader>(ph);
        msg = Utils::addHeader(sph, msg);
        sensorGate->Send(msg);
        
    }
}

