#include "DataSerializer.h"

DataSerializer::DataSerializer(SocketInterface* _si): si(_si){}

void DataSerializer::Send(std::string msg){
    TypeHeader th = {0};
    std::vector<char> data = Utils::serializeString(msg);
    si->Send(Utils::addHeader<TypeHeader>(th, data));
}

void DataSerializer::Send(SimpleStruct msg){
    TypeHeader th = {1};
    msg.a = htons(msg.a);
    std::vector<char> data = Utils::serializeStruct<SimpleStruct>(msg);
    si->Send(Utils::addHeader<TypeHeader>(th, data));
}

std::variant<std::string, SimpleStruct> DataSerializer::Receive(bool echo){
    auto [header, msg] = Utils::divideHeader(1,si->Receive(echo));
    TypeHeader th = Utils::deserializeStruct<TypeHeader>(header);
    switch (th.type)
    {
    case 0:
        return HandleString(msg);
    case 1:
        return HandleStruct(msg);
    default:
        return "Error: type of message unknown";
    }
}

std::vector<std::variant<std::string, SimpleStruct>> DataSerializer::ReceiveAll(){
    std::vector<std::variant<std::string, SimpleStruct>> result;
    std::vector<std::vector<char>> incoming = si->ReceiveAll();
    for(int i = 0; i < incoming.size(); i++){
        auto [header, msg] = Utils::divideHeader(1, incoming[i]);
        TypeHeader th = Utils::deserializeStruct<TypeHeader>(header);
        switch (th.type)
        {
        case 0:
            result.push_back(HandleString(msg));
            break;
        case 1:
            result.push_back(HandleStruct(msg));
            break;
        default:
            result.push_back(std::string("Error: type of message unknown"));
            break;
        }
    }
    return result;
}

SimpleStruct DataSerializer::HandleStruct(std::vector<char> data){
    SimpleStruct msg =  Utils::deserializeStruct<SimpleStruct>(data);
    msg.a = ntohs(msg.a);
    return msg;
}

std::string DataSerializer::HandleString(std::vector<char> data){
    return Utils::deserializeString(data);        
}
