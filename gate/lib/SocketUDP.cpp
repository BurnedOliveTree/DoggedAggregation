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
        sock.SendToKnown(Utils::addHeader(Utils::serializeStruct<PHeader>(ph), splited_msg[i]), ChooseClient(msg));
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
    if(client_history.count(sh.documentType)==0){
        client_history[sh.documentType] = data.second;
    }
}

sockaddr SocketUDP::ChooseClient(std::vector<char> data){
    auto [sh_raw, _] = Utils::divideHeader(sizeof(DocumentHeader), data);
    auto sh = Utils::deserializeStruct<DocumentHeader>(sh_raw);
    if(client_history.count(sh.documentType)>0){
        return client_history[sh.documentType];
    }
    std::runtime_error("Trying to send data to client whoose adress is not known");
}




