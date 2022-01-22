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
int nServers =4;

int main(int argc, char *argv[]) {
    if (argc == 2){
        ipAdress = std::string(argv[1]);
    }
    std::cout <<"IP: " << ipAdress << std::endl;
    std::atomic<bool> isProgramRunning = true;
    auto gate = Gate(ipAdress, port, nServers);
    auto timer = &Timer::getInstance();
    std::vector<char> raw;
    
    while (isProgramRunning) {
        raw = gate.sensorGate->ReceiveRaw(true);
        auto [ph_raw, msg] = Utils::divideHeader(sizeof(PHeader), raw);
        auto [sh_raw, data] = Utils::divideHeader(sizeof(DocumentHeader), msg);

        auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
        auto ph = Utils::deserializeStruct<PHeader>(ph_raw);
        Utils::printVector(raw);
        std::cout << "Document Header: \t" << unsigned(sh.documentId) << "\t" << unsigned(sh.documentType) << "\t" << unsigned(sh.type) << "\n";
        std::cout << "PHeader: \t\t" << ntohs(ph.current) << "\t" << ntohs(ph.total) << "\t" << ntohs(ph.timestamp) << "\n";

        if(gate.AgregateData(sh.documentType,sh.documentId,ntohs(ph.current),ntohs(ph.total),data,ntohs(ph.timestamp))){
            std::vector<char> msg = gate.ConstructDocumentMsg(sh.documentType,sh.documentId);
            Utils::printVector(msg);
            // gate.serwerGate[sh.documentType]->Send(msg);
        }
    }
}