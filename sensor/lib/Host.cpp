#include "Host.h"

Host::Host(SocketInterface* _si): serializer(_si){}

void Host::send(std::variant<std::string, SimpleStruct> msg){
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if(srg_msg){
        serializer.send(*srg_msg);
    }
    SimpleStruct* stc_msg = std::get_if<SimpleStruct>(&msg);
    if(stc_msg){
        serializer.send(*stc_msg);
    }
}

std::variant<std::string, SimpleStruct> Host::receive(){
    return serializer.receive();
}