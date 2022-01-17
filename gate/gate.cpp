#include "lib/Gate.h"
#include "lib/Timer.h"
#include <iostream>
#include <array>
#include <atomic>
#include <memory>
#include <variant>
#include <thread>

int main() {
    std::atomic<bool> isProgramRunning = true;
    auto gate = Gate();
    auto timer = &Timer::getInstance();
    std::vector<char> raw;
    
    while (isProgramRunning) {
        raw = gate.sensorGate->ReceiveRaw(true);
        Utils::printVector(raw);
        auto [ph_raw, msg] = Utils::divideHeader(sizeof(PHeader), raw);
        auto [sh_raw, data] = Utils::divideHeader(sizeof(DocumentHeader), msg);

        auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
        auto ph = Utils::deserializeStruct<PHeader>(ph_raw);
        if(gate.AgregateData(sh.documentType,sh.documentId,ntohs(ph.current),ntohs(ph.total),data,ntohs(ph.timestamp))){
            std::vector<char> msg = gate.ConstructDocumentMsg(sh.documentType,sh.documentId);
            Utils::printVector(msg);
            gate.serwerGate[sh.documentType]->Send(msg);
        }

    }
}