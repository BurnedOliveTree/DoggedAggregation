#include "Host.h"

Host::Host(SocketInterface* si) {
    socketInterface = si;
}

void Host::exchange(std::variant<std::string, uint16_t> msg, uint16_t documentId, uint8_t documentType) {
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if (srg_msg) {
        socketInterface->exchange(DataSerializer::serialize(*srg_msg, documentId, documentType));
    }
    uint16_t* stc_msg = std::get_if<uint16_t>(&msg);
    if (stc_msg) {
        socketInterface->exchange(DataSerializer::serialize(*stc_msg, documentId, documentType));
    }
}

void Host::send(std::variant<std::string, uint16_t> msg, uint16_t documentId, uint8_t documentType) {
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if (srg_msg) {
        socketInterface->send(DataSerializer::serialize(*srg_msg, documentId, documentType));
    }
    uint16_t* stc_msg = std::get_if<uint16_t>(&msg);
    if (stc_msg) {
        socketInterface->send(DataSerializer::serialize(*stc_msg, documentId, documentType));
    }
}

std::variant<std::string, uint16_t> Host::receive() {
    return DataSerializer::deserialize(socketInterface->receive());
}

uint16_t Host::receiveTime() {
    while (true) {
        auto result = DataSerializer::deserialize(socketInterface->receive(MSG_PEEK));
        if (std::get_if<uint16_t>(&result)) {
            socketInterface->receive();
            return std::get<uint16_t>(result);
        }
    }
}
