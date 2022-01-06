#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include <iostream>
#include <vector>

void printVariant(std::variant<std::string, SimpleStruct> msg){
    if (std::get_if<std::string>(&msg)) {
        std::cout << "[sensor.cpp:8] Received string message: " << std::get<std::string>(msg) << std::endl;
    }
    if (std::get_if<SimpleStruct>(&msg)) {
        std::cout << "[sensor.cpp:11] Received struct message: " << std::get<SimpleStruct>(msg).a << " " << unsigned(std::get<SimpleStruct>(msg).b) << " " << unsigned(std::get<SimpleStruct>(msg).c) << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> messages {"abc1", "0001", "hello", "world", "QUIT", "Hmmm?"};
    SocketInterface* socketInterface;
    Host* client;

    if (argc > 2) {
        socketInterface = new SocketUDP(argv[1], atoi(argv[2]));
    } else {
        socketInterface = new SocketUDP();
    }
    client = new Host(socketInterface);
    std::cout << "[sensor.cpp:27] Initialized main variables" << std::endl;

    for (auto & message : messages) {
        try {
            client->send(message);
            printVariant(client->receive());
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    delete client;
    delete socketInterface;
}
