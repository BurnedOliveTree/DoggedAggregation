#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <thread>


bool run_program = true;

void generateData(std::queue<std::string>* dataStream) {
    uint64_t counter = 0;
    std::cout << "[sensor.cpp:13] Started to generate data" << std::endl;
    while (run_program) {
        std::cout << "[sensor.cpp:15] Generated data" << std::endl;
        dataStream -> push("random number: "+std::to_string(counter));
        sleep(rand() % 5);
        counter++;
        if (counter == 20)
            run_program = false;
    }
}

int main(int argc, char* argv[])
{
    auto messages = new std::queue<std::string>();
    SocketInterface* socketInterface;
    Host* client;

    std::thread thread_obj(generateData, messages);
    if (argc > 2) {
        socketInterface = new SocketUDP(argv[1], atoi(argv[2]));
    } else {
        socketInterface = new SocketUDP();
    }
    client = new Host(socketInterface);
    std::cout << "[sensor.cpp:37] Initialized main variables" << std::endl;

    while (run_program)
    {
        try {
            if (!messages->empty()) {
                client->exchange(messages->front());
                messages->pop();
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    delete messages;
    delete client;
    delete socketInterface;
}
