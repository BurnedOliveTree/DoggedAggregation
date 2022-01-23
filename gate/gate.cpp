#include "lib/Gate.h"
#include "lib/Timer.h"
#include <iostream>
#include <array>
#include <atomic>
#include <memory>
#include <variant>
#include <thread>

std::string ipAdress = "127.0.0.1";
int port = 8000;
int nServers =3;
std::atomic<bool> isProgramRunning = true;


int main(int argc, char *argv[]) {
    if (argc == 2){
        ipAdress = std::string(argv[1]);
    }
    std::cout <<"IP: " << ipAdress << std::endl;
    auto gate = Gate(ipAdress, port, nServers);
    auto timer = &Timer::getInstance();
    int last_sync = 0;
    std::vector<char> raw;
    
    
    while (isProgramRunning) {
        raw = gate.sensorGate->ReceiveRaw(true);
        
        auto [ph_raw, msg] = Utils::divideHeader(sizeof(PHeader), raw);
        auto [sh_raw, data] = Utils::divideHeader(sizeof(DocumentHeader), msg);

        auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
        auto ph = Utils::deserializeStruct<PHeader>(ph_raw);
        std::cout << "\nNew data arrived:\n";
        std::cout << "Document Header: \t" << unsigned(sh.documentId) << "\t" << unsigned(sh.documentType) << "\t" << unsigned(sh.type) << "\n";
        std::cout << "PHeader: \t\t" << ntohs(ph.current) << "\t" << ntohs(ph.total) << "\t" << ntohs(ph.timestamp) << "\n";

        if(gate.AgregateData(sh.documentType,sh.documentId,ntohs(ph.current),ntohs(ph.total),data,ntohs(ph.timestamp))){
            std::cout << "\nData is agregated and ready for server " << unsigned(sh.documentType) << std::endl;
            std::vector<char> msg = gate.ConstructDocumentMsg(sh.documentType,sh.documentId);
            auto [hed, _] = Utils::divideHeader(sizeof(AgregatedHeader), msg);
            Utils::printVector(hed);
            gate.serwerGate[sh.documentType]->Send(msg);
        }
        if(timer->getCounter() - last_sync >= 5){
            gate.SynchronizeTime();
            last_sync = timer->getCounter();
        }
    }
    std::cout<< "Ending safely"; 
}