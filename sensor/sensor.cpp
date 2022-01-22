#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include "lib/Timer.h"
#include "lib/Document.h"
#include <iostream>
#include <thread>

std::string ipAdress = "127.0.0.1";
int port = 8000;

void timeSynchronization(Host* client, std::atomic<bool> isProgramRunning) {
    auto timer = &Timer::getInstance();
    while (isProgramRunning) {
        auto newTime = client->receiveTime();
        auto latency = Utils::ping() / timer->tick;
        timer->setCounter(newTime + latency);
        std::this_thread::sleep_for(timer->tick / 10);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("Please specify type of data! (0-3)");
    }
    if (argc == 3){
        ipAdress = std::string(argv[2]);
    }
    std::cout <<"IP: " << ipAdress << std::endl;
    std::atomic<bool> isProgramRunning = true;
    auto timer = &Timer::getInstance();
    auto messages = DocumentContainer(isProgramRunning, static_cast<DocumentType>(atoi(argv[1])));
    SocketInterface* socketInterface;
    Host* client;

    auto config = Utils::readConfig();
    socketInterface = new SocketUDP(ipAdress, port);
    client = new Host(socketInterface);
    std::thread timeThread(&timeSynchronization, client, &isProgramRunning);
    std::cout << "[sensor.cpp:30] Initialized main variables" << std::endl;

    while (isProgramRunning) {
        if (!messages.isEmpty()) {
            auto currentMessage = messages.get();
            client->exchange(currentMessage.message, currentMessage.documentId, currentMessage.documentType);
        }
        std::this_thread::sleep_for(timer->tick);
    }

    delete client;
    delete socketInterface;
}
