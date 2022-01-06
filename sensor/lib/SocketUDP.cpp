#include "SocketUDP.h"

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {}

void SocketUDP::send(std::vector<char> msg){
    std::vector<std::vector<char>> splitMessage = Utils::splitData(msg, MAX_PACKET_SIZE - 4);
    for (uint8_t i = 0; i < splitMessage.size(); i++)
    {
        Utils::printVector(splitMessage[i]);
        PacketHeader packetHeader = {htons(splitMessage[i].size()), static_cast<uint8_t>(splitMessage.size()), i};
        sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
    }
}

std::vector<char> SocketUDP::receive(){
    std::vector<char> result;
    std::vector<char> rec;
    PacketHeader packetHeader;
    do {
        rec = sock.receive();
        Utils::printVector(rec);
        auto [hd, msg] = Utils::divideHeader(4, rec);
        // Utils::printVector(msg);
        packetHeader = Utils::deserializeStruct<PacketHeader>(hd);
        result.insert(result.end(), msg.begin(), msg.end());
    } while(packetHeader.max_packet - packetHeader.curr_packet > 1);
    return result;
}


