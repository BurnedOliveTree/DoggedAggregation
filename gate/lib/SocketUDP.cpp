#include "SocketUDP.h"

SocketUDP::SocketUDP(std::string ip, int port,  bool is_serv): sock(ip,port,is_serv,true){
    if(is_serv){
        sock.Bind();
    }
}

void SocketUDP::Send(std::vector<char> msg){
    std::vector<std::vector<char>> splited_msg = Utils::splitData(msg, MAX_PACKET_SIZE-sizeof(PHeader));
    for(uint8_t i = 0; i < splited_msg.size(); i++)
    {
        Utils::printVector(splited_msg[i]);
        PHeader ph = {htons(splited_msg[i].size()), static_cast<uint8_t>(splited_msg.size()), i};
        sock.Send(Utils::addHeader(Utils::serializeStruct<PHeader>(ph), splited_msg[i]));
    }
}

std::vector<char> SocketUDP::Receive(bool echo){
    std::vector<char> result, rec;
    PHeader ph;
    rec = sock.Receive();
    auto [hd, msg] = Utils::divideHeader(sizeof(PHeader), rec);
    ph = Utils::deserializeStruct<PHeader>(hd);
    result.insert(result.end(), msg.begin(), msg.end());
    if(echo){
        sock.Send(rec);
    }
    return result;
}

std::vector<char> SocketUDP::ReceiveRaw(bool echo){
    std::vector<char> rec;
    PHeader ph;
    rec = sock.Receive();
    if(echo){
        auto [hd, msg] = Utils::divideHeader(sizeof(PHeader), rec);
        ph = Utils::deserializeStruct<PHeader>(hd);
        sock.Send(Utils::serializeStruct(ph));
    }
    return rec;
}




