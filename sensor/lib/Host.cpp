#include "Host.h"

Host::Host(SocketInterface* si) {
    socketInterface = si;
}

void Host::send(std::variant<std::string, uint16_t> msg, uint16_t documentId, uint8_t documentType) {
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if (srg_msg) {
        auto [header, body] = DataSerializer::serialize(*srg_msg, documentId, documentType);
        socketInterface->send(header, body);
    }
    uint16_t* stc_msg = std::get_if<uint16_t>(&msg);
    if (stc_msg) {
        auto [header, body] = DataSerializer::serialize(*stc_msg, documentId, documentType);
        socketInterface->send(header, body);
    }
}

std::variant<std::string, uint16_t> Host::receive(uint8_t type) {
    return DataSerializer::deserialize(socketInterface->receive(type));
}
