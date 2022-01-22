#include "SocketUDP.h"
#include <iostream>

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {
    timeoutAfter = std::stoul(Utils::readConfig()["timeoutAfter"]);
}

bool SocketUDP::isACK(PacketHeader send, PacketHeader received, StandardHeader send2, StandardHeader received2) {
    return send.current == received.current &&
        send.total == received.total &&
        send.timestamp == received.timestamp &&
        send2.documentType == received2.documentType &&
        send2.documentId == received2.documentId;
}

void SocketUDP::exchange(StandardHeader sendStandardHeader, std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - sizeof(PacketHeader));
    uint32_t totalPacketAmount = splitMessage.size();
    std::vector<char> datagram;
    std::vector<char> sendDatagram;
    PacketHeader sendPacketHeader, receivedPacketHeader;
    StandardHeader receivedStandardHeader;
    Timer* timer = &Timer::getInstance();
    uint16_t count[2];
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        count[1] = timer->getCounter();
        sendPacketHeader = {htons(timer->getCounter()), htons(totalPacketAmount), htons(i)};
        sendDatagram =  Utils::addHeader(Utils::serializeStruct<PacketHeader>(sendPacketHeader), Utils::addHeader(Utils::serializeStruct<StandardHeader>(sendStandardHeader), splitMessage[i]));
        do {
            count[0] = timer->getCounter();
            do {
                Utils::printVector(sendDatagram, "Sent datagram");
                sock.send(sendDatagram);
            } while (!sock.isDataPresent() && (timer->getCounter() - count[0]) < timeoutAfter);
            datagram = sock.receive();
            Utils::printVector(datagram, "Received datagram");
            auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
            receivedPacketHeader = Utils::deserializeStruct<PacketHeader>(header);
            auto[secondHeader, secondBody] = Utils::divideHeader(sizeof(StandardHeader), body);
            receivedStandardHeader = Utils::deserializeStruct<StandardHeader>(secondHeader);
        } while (!isACK(sendPacketHeader, receivedPacketHeader, sendStandardHeader, receivedStandardHeader) && (timer->getCounter() - count[1]) < timeoutAfter);
    }
}

void SocketUDP::send(std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - sizeof(PacketHeader));
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


