#include "SocketUDP.h"

SocketUDP::SocketUDP(std::string ip, int port,  bool is_serv): sock(ip,port,is_serv,true){
    if(is_serv){
        sock.Bind();
    }
}

void SocketUDP::Send(std::vector<char> msg){
    sockaddr s = ChooseClient(msg);
    std::vector<std::vector<char>> splited_msg = Utils::splitData(msg, MAX_PACKET_SIZE-sizeof(PHeader));
    for(uint8_t i = 0; i < splited_msg.size(); i++)
    {
        sock.SendToKnown(splited_msg[i], s);
    }
}

std::vector<char> SocketUDP::Receive(bool echo){
    std::pair<std::vector<char>, sockaddr> buff;
    std::vector<char> result, rec;
    PHeader ph;
    buff = sock.ReceiveWithSender();
    RememberClient(buff);
    rec = buff.first;
    auto [hd, msg] = Utils::divideHeader(sizeof(PHeader), rec);
    ph = Utils::deserializeStruct<PHeader>(hd);
    result.insert(result.end(), msg.begin(), msg.end());
    if(echo){
        sock.SendToKnown(rec, buff.second);
    }
    return result;
}

std::vector<char> SocketUDP::ReceiveRaw(bool echo){
    std::pair<std::vector<char>, sockaddr> buff;
    std::vector<char> rec;
    buff = sock.ReceiveWithSender();
    RememberClient(buff);
    rec = buff.first;
    if(echo){
        auto [hd, msg] = Utils::divideHeader(sizeof(PHeader), rec);
        sock.SendToKnown(rec, buff.second);
    }
    return rec;
}

void SocketUDP::RememberClient(std::pair<std::vector<char>,sockaddr> data){
    auto [ph_raw, msg] = Utils::divideHeader(sizeof(PHeader), data.first);
    auto [sh_raw, _] = Utils::divideHeader(sizeof(DocumentHeader), msg);
    auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
    client_history.erase(sh.documentType);
    client_history.insert(std::make_pair(sh.documentType, data.second));
    
}

sockaddr SocketUDP::ChooseClient(std::vector<char> data){
    auto [ph_raw, msg] = Utils::divideHeader(sizeof(PHeader), data);
    auto [sh_raw, _] = Utils::divideHeader(sizeof(DocumentHeader), msg);
    auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
    if(client_history.count(sh.documentType)==1){
        return client_history[sh.documentType];
    }
}




