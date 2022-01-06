#include "DataSerializer.h"

DataSerializer::DataSerializer(SocketInterface* _si): socketInterface(_si){}

void DataSerializer::send(std::string msg){
    TypeHeader typeHeader = {0};
    std::vector<char> data = Utils::serializeString(msg);
    socketInterface->send(Utils::addHeader<TypeHeader>(typeHeader, data));
}

void DataSerializer::send(SimpleStruct msg){
    TypeHeader typeHeader = {1};
    msg.a = htons(msg.a);
    std::vector<char> data = Utils::serializeStruct<SimpleStruct>(msg);
    socketInterface->send(Utils::addHeader<TypeHeader>(typeHeader, data));
}

std::variant<std::string, SimpleStruct> DataSerializer::receive(){
    auto [header, msg] = Utils::divideHeader(1, socketInterface->receive());
    auto typeHeader = Utils::deserializeStruct<TypeHeader>(header);
    switch (typeHeader.type)
    {
    case 0:
        return handleString(msg);
    case 1:
        return handleStruct(msg);
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
