#include "SocketUDP.h"
#include <iostream>

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {
    timeoutAfter = std::stoul(Utils::readConfig()["timeoutAfter"]);
}

void SocketUDP::exchange(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - 4);
    uint32_t totalPacketAmount = splitMessage.size();
    std::vector<char> datagram;
    PacketHeader packetHeader;
    Timer* timer = &Timer::getInstance();
    uint16_t count[2];
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        count[1] = timer->getCounter();
        do {
            count[0] = timer->getCounter();
            do {
                Utils::printVector(splitMessage[i], "Sent datagram");
                packetHeader = {htons(timer->getCounter()), htons(totalPacketAmount), htons(i)};
                sock.send(Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), splitMessage[i]));
            } while (!sock.isDataPresent() && (timer->getCounter() - count[0]) < timeoutAfter);
            std::cout << "TUTAJ!!" << std::endl;
            datagram = sock.receive();
            Utils::printVector(datagram, "Received datagram");
            auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
            packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        } while (packetHeader.current < i && (timer->getCounter() - count[1]) < timeoutAfter);
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

std::vector<char> SocketUDP::receive(int flag) {
    std::vector<char> result;
    std::vector<char> datagram;
    PacketHeader packetHeader;
    do {
        datagram = sock.receive(flag);
        if (flag != MSG_PEEK)
            Utils::printVector(datagram, "Received datagram");
        auto [header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
        packetHeader = Utils::deserializeStruct<PacketHeader>(header);
        result.insert(result.end(), body.begin(), body.end());
    } while(packetHeader.total - packetHeader.current > 1);
    return result;
}


