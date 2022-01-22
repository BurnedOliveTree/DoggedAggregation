#include "SocketTCP.h"


SocketTCP::SocketTCP(std::string ip, int port,  bool is_serv): sock(ip,port,is_serv,false){
    if(is_serv){
        sock.Bind();
        sock.Listen();
    }
    else{
        sock.Connect();
    }
}

void SocketTCP::Send(std::vector<char> msg){
    std::vector<std::vector<char>> splited_msg = Utils::splitData(msg, MAX_PACKET_SIZE-sizeof(THeader));
    for(int i = 0; i < splited_msg.size(); i++)
    {
        THeader ph = { htons(static_cast<uint16_t>(splited_msg.size())),htons(static_cast<uint16_t>(i)), htonl(static_cast<uint32_t>(splited_msg[i].size())) };
        sock.Write(Utils::addHeader(Utils::serializeStruct<THeader>(ph), splited_msg[i]));
    }
}

std::vector<char> SocketTCP::Receive(bool echo){
    std::vector<char> result, rec;
    THeader ph;
    do{
        std::vector<char> hd = sock.Read(4);
        Utils::printVector(hd);
        ph = Utils::deserializeStruct<THeader>(hd);
        std::vector<char> msg = sock.Read(ph.size);
        Utils::printVector(msg);
        result.insert(result.end(), msg.begin(), msg.end());
    } while(ph.total-ph.current>1);
    return result;
}

std::vector<char> SocketTCP::ReceiveRaw(bool echo){
    std::vector<char> result, rec;
    THeader ph;
    do{
        std::vector<char> hd = sock.Read(4);
        Utils::printVector(hd);
        ph = Utils::deserializeStruct<THeader>(hd);
        std::vector<char> msg = sock.Read(ph.size);
        Utils::printVector(msg);
        result.insert(result.end(), msg.begin(), msg.end());
    } while(ph.total-ph.current>1);
    return result;
}
