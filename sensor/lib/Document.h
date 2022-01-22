#pragma once

#include <cstdint>
#include <string>
#include <queue>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>


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

class DocumentContainer {
    std::queue<Document> dataContainer;
    std::atomic<bool>& isProgramRunning;
    std::mutex internalMutex;
    std::thread internalThread;
    DocumentType type;

    void generateData();
public:
    explicit DocumentContainer(std::atomic<bool> &isProgramRunning, DocumentType type);
    bool isEmpty();
    Document get();
};