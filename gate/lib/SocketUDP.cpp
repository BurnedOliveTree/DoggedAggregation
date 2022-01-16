#include "SocketUDP.h"

SocketUDP::SocketUDP(std::string ip, int port,  bool is_serv): sock(ip,port,is_serv,true){
    if(is_serv){
        sock.Bind();
    }
}

void SocketUDP::Send(std::vector<char> msg){
    std::vector<std::vector<char>> splited_msg = Utils::splitData(msg, MAX_PACKET_SIZE-4);
    for(uint8_t i = 0; i < splited_msg.size(); i++)
    {
        Utils::printVector(splited_msg[i]);
        PacketHeaderUDP ph = {htons(splited_msg[i].size()), static_cast<uint8_t>(splited_msg.size()), i};
        sock.Send(Utils::addHeader(Utils::serializeStruct<PacketHeaderUDP>(ph), splited_msg[i]));
    }
}

std::vector<char> SocketUDP::Receive(bool echo ){
    std::vector<char> result, rec;
    PacketHeaderUDP ph;
    rec = sock.Receive();
    auto [hd, msg] = Utils::divideHeader(sizeof(ph), rec);
    ph = Utils::deserializeStruct<PacketHeaderUDP>(hd);
    result.insert(result.end(), msg.begin(), msg.end());
    if(echo){
        Utils::printVector(result);
        sock.Send(result);
    }
    return result;
}

std::vector<std::vector<char>> SocketUDP::ReceiveAll(){
    std::vector<std::vector<char>> result;
    result.push_back(Receive());
    return result;
}



