#include "DataSerializer.h"

DataSerializer::DataSerializer() = default;

std::vector<char> DataSerializer::serialize(std::string message, uint16_t documentId, uint8_t documentType){
    StandardHeader typeHeader = { documentId, documentType, 0};
    std::vector<char> data = Utils::serializeString(message);
    return Utils::addHeader<StandardHeader>(typeHeader, data);
}

std::vector<char> DataSerializer::serialize(uint16_t time, uint16_t documentId, uint8_t documentType){
    StandardHeader typeHeader = { documentId, documentType, 1};
    std::vector<char> data = Utils::serializeTime(time);
    return Utils::addHeader<StandardHeader>(typeHeader, data);
}

std::variant<std::string, uint16_t> DataSerializer::deserialize(std::vector<char> data){
    auto [header, message] = Utils::divideHeader(sizeof(StandardHeader), data);
    auto typeHeader = Utils::deserializeStruct<StandardHeader>(header);
    switch (typeHeader.type) {
        case 0:
            return handleString(message);
        case 1:
            return handleTime(message);
        default:
            return "Error: type of message unknown";
    }
}

uint16_t DataSerializer::handleTime(std::vector<char> data){
    return Utils::deserializeTime(data);
}

std::string DataSerializer::handleString(std::vector<char> data){
    return Utils::deserializeString(data);        
}

void DataSerializer::printVariant(std::variant<std::string, uint16_t> msg) {
    if (std::get_if<std::string>(&msg)) {
        std::cout << "[DataSerializer.cpp:40] Received string message: " << std::get<std::string>(msg) << std::endl;
    }
    if (std::get_if<uint16_t>(&msg)) {
        std::cout << "[DataSerializer.cpp:43] Received time message: " << std::get<uint16_t>(msg) << std::endl;
    }
}
