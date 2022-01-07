#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include "lib/Timer.h"
#include "lib/Document.h"
#include <iostream>
#include <regex>

std::chrono::nanoseconds ping() {
    char buffer[256];
    auto file = popen("ping 127.0.0.1 -c 1", "r");
    fgets(buffer, 256, file);
    fgets(buffer, 256, file);
    pclose(file);

    std::smatch result[3];
    auto temp = std::string(buffer);
    std::regex_search(temp, result[0], std::regex("time=.*"));
    temp = std::string(result[0][0]);
    std::regex_search(temp, result[1], std::regex("[0-9]+\\.[0-9]+"));
    std::regex_search(temp, result[2], std::regex("[mn]?s"));

    double value;
    if (result[2][0] == "ms") {
        value = 1000000 * stod(result[1][0]);
    } else if(result[2][0] == "s") {
        value = stod(result[1][0]);
    } else {
        throw std::invalid_argument("Ping returned a value in other units than: ms, s!");
    }
    return std::chrono::nanoseconds(static_cast<long long>(value));
}

void timeSynchronization(Host* client, std::atomic<bool> isProgramRunning) {
    auto timer = &Timer::getInstance();
    while (isProgramRunning) {
        auto newTime = client->receiveTime();
        auto latency = ping() / TICK;
        timer->setCounter(newTime + latency);
        std::this_thread::sleep_for(TICK / 10);
    }
}

int main(int argc, char* argv[])
{
    // TODO: add configuration
    std::atomic<bool> isProgramRunning = true;
    auto timer = &Timer::getInstance();
    auto messages = DocumentContainer(isProgramRunning);
    SocketInterface* socketInterface;
    Host* client;

    if (argc > 2) {
        socketInterface = new SocketUDP(argv[1], atoi(argv[2]));
    } else {
        socketInterface = new SocketUDP("127.0.0.1" , 8000);
    }
    client = new Host(socketInterface);
    std::thread timeThread(&timeSynchronization, client, &isProgramRunning);
    std::cout << "[sensor.cpp:88] Initialized main variables" << std::endl;

    while (isProgramRunning)
    {
        if (!messages.isEmpty()) {
            auto currentMessage = messages.get();
            client->exchange(currentMessage.message, currentMessage.documentId, currentMessage.documentType);
        }
        std::this_thread::sleep_for(TICK);
    }

    delete client;
    delete socketInterface;
}
