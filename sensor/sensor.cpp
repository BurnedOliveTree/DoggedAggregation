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

void printVariant(std::variant<std::string, SimpleStruct> msg) {
    if (std::get_if<std::string>(&msg)) {
        std::cout << "[sensor.cpp:26] Received string message: " << std::get<std::string>(msg) << std::endl;
    }
    if (std::get_if<SimpleStruct>(&msg)) {
        std::cout << "[sensor.cpp:29] Received struct message: " << std::get<SimpleStruct>(msg).a << " " << unsigned(std::get<SimpleStruct>(msg).b) << " " << unsigned(std::get<SimpleStruct>(msg).c) << std::endl;
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
    std::cout << "[sensor.cpp:46] Initialized main variables" << std::endl;

    while (run_program)
    {
        try {
            if (!messages->empty()) {
                client->send(messages->front());
                messages->pop();
                printVariant(client->receive());
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    delete messages;
    delete client;
    delete socketInterface;
}
