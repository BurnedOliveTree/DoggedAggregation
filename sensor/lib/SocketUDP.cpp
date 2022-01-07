#include "SocketUDP.h"

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {}

void SocketUDP::exchange(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - 4);
    uint32_t totalPacketAmount = splitMessage.size();
    std::vector<char> datagram;
    PacketHeader packetHeader;
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        do {
            do {
                Utils::printVector(splitMessage[i]);
                packetHeader = {htons(2137), htons(totalPacketAmount), htons(i)};    // TODO change 2137 to a meaningful timestamp
                sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
            } while (!sock.isDataPresent());
            datagram = sock.receive();
            Utils::printVector(datagram);
            auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
            packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        } while (packetHeader.current < i);
    }
}

void SocketUDP::send(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - 4);
    uint32_t totalPacketAmount = splitMessage.size();
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        Utils::printVector(splitMessage[i]);
        PacketHeader packetHeader = {htons(2137), htons(totalPacketAmount), htons(i)};    // TODO change 2137 to a meaningful timestamp
        sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
    }
}

std::vector<char> SocketUDP::receive(){
    std::vector<char> result;
    std::vector<char> datagram;
    PacketHeader packetHeader;
    do {
        datagram = sock.receive();
        Utils::printVector(datagram);
        auto [header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
        packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        result.insert(result.end(), body.begin(), body.end());
    } while(packetHeader.total - packetHeader.current > 1);
    return result;
}


