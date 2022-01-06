#include "Host.h"

Host::Host(SocketInterface* si) {
    socketInterface = si;
}

void Host::exchange(std::variant<std::string, SimpleStruct> msg) {
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if (srg_msg) {
        socketInterface->exchange(DataSerializer::serialize(*srg_msg));
    }
    SimpleStruct* stc_msg = std::get_if<SimpleStruct>(&msg);
    if (stc_msg) {
        socketInterface->exchange(DataSerializer::serialize(*stc_msg));
    }
}

void Host::send(std::variant<std::string, SimpleStruct> msg){
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if (srg_msg) {
        socketInterface->send(DataSerializer::serialize(*srg_msg));
    }
    SimpleStruct* stc_msg = std::get_if<SimpleStruct>(&msg);
    if (stc_msg) {
        socketInterface->send(DataSerializer::serialize(*stc_msg));
    }
}

std::variant<std::string, SimpleStruct> Host::receive(){
    return DataSerializer::deserialize(socketInterface->receive());
}
