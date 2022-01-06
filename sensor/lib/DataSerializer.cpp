#include "DataSerializer.h"

DataSerializer::DataSerializer() = default;

std::vector<char> DataSerializer::serialize(std::string message){
    TypeHeader typeHeader = {0};
    std::vector<char> data = Utils::serializeString(message);
    return Utils::addHeader<TypeHeader>(typeHeader, data);
}

std::vector<char> DataSerializer::serialize(SimpleStruct message){
    TypeHeader typeHeader = {1};
    message.a = htons(message.a);
    std::vector<char> data = Utils::serializeStruct<SimpleStruct>(message);
    return Utils::addHeader<TypeHeader>(typeHeader, data);
}

std::variant<std::string, SimpleStruct> DataSerializer::deserialize(std::vector<char> data){
    auto [header, message] = Utils::divideHeader(sizeof(TypeHeader), data);
    auto typeHeader = Utils::deserializeStruct<TypeHeader>(header);
    switch (typeHeader.type) {
        case 0:
            return handleString(message);
        case 1:
            return handleStruct(message);
        default:
            return "Error: type of message unknown";
    }
}

SimpleStruct DataSerializer::handleStruct(std::vector<char> data){
    auto message = Utils::deserializeStruct<SimpleStruct>(data);
    message.a = ntohs(message.a);
    return message;
}

std::string DataSerializer::handleString(std::vector<char> data){
    return Utils::deserializeString(data);        
}

void DataSerializer::printVariant(std::variant<std::string, SimpleStruct> msg) {
    if (std::get_if<std::string>(&msg)) {
        std::cout << "[sensor.cpp:26] Received string message: " << std::get<std::string>(msg) << std::endl;
    }
    if (std::get_if<SimpleStruct>(&msg)) {
        std::cout << "[sensor.cpp:29] Received struct message: " << std::get<SimpleStruct>(msg).a << " " << unsigned(std::get<SimpleStruct>(msg).b) << " " << unsigned(std::get<SimpleStruct>(msg).c) << std::endl;
    }
}
