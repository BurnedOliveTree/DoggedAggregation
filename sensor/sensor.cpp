#include "lib/SocketUDP.h"
#include "lib/Host.h"
#include "lib/Timer.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <thread>
#include <regex>

bool run_program = true;
std::mutex dataStreamMutex;

enum DocumentType {
    JPG,
    PNG,
    TXT,
    DOC
};

struct Document {
    Document(std::string argMessage, uint16_t argDocumentId, DocumentType argDocumentType) {
        message = argMessage;
        documentId = argDocumentId;
        documentType = argDocumentType;
    }

    uint16_t documentId;
    uint8_t documentType;
    std::string message;
};

void generateData(std::queue<Document>* dataContainer) {
    uint64_t counter = 0;
    std::cout << "[sensor.cpp:34] Started to generate data" << std::endl;
    while (run_program) {
        std::cout << "[sensor.cpp:36] Generated data" << std::endl;
        dataStreamMutex.lock();
        dataContainer -> push(Document("random number: " + std::to_string(counter), counter / 10, TXT));
        dataStreamMutex.unlock();
        std::this_thread::sleep_for(std::chrono::nanoseconds(random() % 4000000000 + 1000000000));
        counter++;
        if (counter == 20)
            run_program = false;
    }
}

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

int main(int argc, char* argv[])
{
    // TODO: add receiving and processing of time synchronization datagrams
    // TODO: add configuration
    auto timer = &Timer::getInstance();
    auto messages = new std::queue<Document>();
    SocketInterface* socketInterface;
    Host* client;

    std::thread thread_obj(generateData, messages);
    if (argc > 2) {
        socketInterface = new SocketUDP(argv[1], atoi(argv[2]));
    } else {
        socketInterface = new SocketUDP("127.0.0.1" , 8000);
    }
    client = new Host(socketInterface);
    std::cout << "[sensor.cpp:88] Initialized main variables" << std::endl;

    while (run_program)
    {
        if (!messages->empty()) {
            dataStreamMutex.lock();
            auto currentMessage = messages->front();
            client->exchange(currentMessage.message, currentMessage.documentId, currentMessage.documentType);
            messages->pop();
            dataStreamMutex.unlock();
        }
    }

    delete messages;
    delete client;
    delete socketInterface;
}
