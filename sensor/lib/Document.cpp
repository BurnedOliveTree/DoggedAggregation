#include "Document.h"

DocumentContainer::DocumentContainer(std::atomic<bool> &isProgramRunning, DocumentType argType): isProgramRunning(isProgramRunning) {
    dataContainer = std::queue<Document>();
    type = argType;
    internalThread = std::thread(&DocumentContainer::generateData, this);
}

void DocumentContainer::generateData() {
    uint64_t counter = 0;
    std::cout << "[sensor.cpp:11] Started to generate data" << std::endl;
    std::string data = "next numbers: ";
    for (int i = 0; i < 300; ++i)
        data += std::to_string(i) + " ";
    while (isProgramRunning) {
        std::cout << "[sensor.cpp:16] Generated data" << std::endl;
        internalMutex.lock();
         dataContainer.push(Document(data, counter, type));

        internalMutex.unlock();
        std::this_thread::sleep_for(std::chrono::nanoseconds(random() % 4000000000 + 1000000000));
        counter++;
        if (counter == 100)
            isProgramRunning = false;
    }
}

bool DocumentContainer::isEmpty() {
    return dataContainer.empty();
}

Document DocumentContainer::get() {
    internalMutex.lock();
    auto currentMessage = dataContainer.front();
    dataContainer.pop();
    internalMutex.unlock();
    return currentMessage;
}
