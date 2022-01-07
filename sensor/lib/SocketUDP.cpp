#include "SocketUDP.h"

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {}

void SocketUDP::exchange(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - 4);
    uint32_t totalPacketAmount = splitMessage.size();
    std::vector<char> datagram;
    PacketHeader packetHeader;
    Timer* timer = &Timer::getInstance();
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        do {
            do {
                Utils::printVector(splitMessage[i], "Sent datagram");
                packetHeader = {htons(timer->getCounter()), htons(totalPacketAmount), htons(i)};
                sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
            } while (!sock.isDataPresent());
            datagram = sock.receive();
            Utils::printVector(datagram, "Received datagram");
            auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
            packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        } while (packetHeader.current < i);
    }
}

void SocketUDP::send(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - 4);
    uint32_t totalPacketAmount = splitMessage.size();
    Timer* timer = &Timer::getInstance();
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        Utils::printVector(splitMessage[i], "Sent datagram");
        PacketHeader packetHeader = {htons(timer->getCounter()), htons(totalPacketAmount), htons(i)};
        sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
    }
}

std::vector<char> SocketUDP::receive(){
    std::vector<char> result;
    std::vector<char> datagram;
    PacketHeader packetHeader;
    do {
        datagram = sock.receive();
        Utils::printVector(datagram, "Received datagram");
        auto [header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
        packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        result.insert(result.end(), body.begin(), body.end());
    } while(packetHeader.total - packetHeader.current > 1);
    return result;
}


