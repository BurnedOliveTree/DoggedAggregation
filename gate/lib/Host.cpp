#include "Host.h"

Host::Host(SocketInterface* _si): ws(_si){}

void Host::Send(std::variant<std::string, SimpleStruct> msg){
    std::string* srg_msg = std::get_if<std::string>(&msg);
    if(srg_msg){
        ws.Send(*srg_msg);
    }
    SimpleStruct* stc_msg = std::get_if<SimpleStruct>(&msg);
    if(stc_msg){
        ws.Send(*stc_msg);
    }
}

std::variant<std::string, SimpleStruct> Host::Receive(bool echo){
    return ws.Receive(echo);
}

std::vector<char> Host::ReceiveRaw(bool echo){
    return ws.ReceiveRaw(echo);
}

std::vector<std::variant<std::string, SimpleStruct>> Host::ReceiveAll(){
    return ws.ReceiveAll();
}