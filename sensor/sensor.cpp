#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include "lib/Timer.h"
#include "lib/Document.h"
#include <iostream>

void timeSynchronization(Host* client, std::atomic<bool> isProgramRunning) {
    auto timer = &Timer::getInstance();
    while (isProgramRunning) {
        auto newTime = client->receiveTime();
        auto latency = Utils::ping() / timer->tick;
        timer->setCounter(newTime + latency);
        std::this_thread::sleep_for(timer->tick / 10);
    }
}

int main() {
    // TODO: fix this stupid "libc++abi: terminating" error
    std::atomic<bool> isProgramRunning = true;
    auto timer = &Timer::getInstance();
    auto messages = DocumentContainer(isProgramRunning);
    SocketInterface* socketInterface;
    Host* client;

    auto config = Utils::readConfig();
    socketInterface = new SocketUDP(config["ipAddress"], std::stoi(config["port"]));
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
