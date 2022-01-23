#include "SocketUDP.h"
#include <iostream>

SocketUDP::SocketUDP(const std::string& ipAddress, int port): sock(ipAddress, port) {
    timeoutAfter = std::stoul(Utils::readConfig()["timeoutAfter"]);
    timer = &Timer::getInstance();
}

bool SocketUDP::isACK(PacketHeader send, StandardHeader send2, std::vector<char> datagram) {
    auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
    auto receivedPacketHeader = Utils::deserializeStruct<PacketHeader>(header);
    auto[secondHeader, secondBody] = Utils::divideHeader(sizeof(StandardHeader), body);
    auto receivedStandardHeader = Utils::deserializeStruct<StandardHeader>(secondHeader);
    return send.current == receivedPacketHeader.current &&
        send.total == receivedPacketHeader.total &&
        send.timestamp == receivedPacketHeader.timestamp &&
        send2.documentType == receivedStandardHeader.documentType &&
        send2.documentId == receivedStandardHeader.documentId;
}

void SocketUDP::send(StandardHeader standardHeader, std::vector<char> message) {
    std::vector<std::vector<char>> splitMessage = Utils::splitData(message, MAX_PACKET_SIZE - sizeof(PacketHeader) - sizeof(StandardHeader));
    uint32_t totalPacketAmount = splitMessage.size();
    std::vector<char> datagram;
    std::vector<char> sendDatagram;
    PacketHeader packetHeader;
    uint16_t sendTime, receiveTime;
    for (uint32_t i = 0; i < totalPacketAmount; i++)
    {
        receiveTime = timer->getCounter();
        packetHeader = {htons(timer->getCounter()), htons(totalPacketAmount), htons(i)};
        sendDatagram =  Utils::addHeader(Utils::serializeStruct<PacketHeader>(packetHeader), Utils::addHeader(Utils::serializeStruct<StandardHeader>(standardHeader), splitMessage[i]));
        do {
            sendTime = timer->getCounter();
            do {
                sock.send(sendDatagram);
                Utils::printVector(sendDatagram, "Sent datagram");
            } while (!sock.isDataPresent() && (timer->getCounter() - sendTime) < timeoutAfter);
            datagram = sock.receive(MSG_PEEK);
            if (datagram[9] == standardHeader.type) // TODO, fix this
                sock.receive();
            Utils::printVector(datagram, "Received datagram");
        } while (!isACK(packetHeader, standardHeader, datagram) && (timer->getCounter() - receiveTime) < timeoutAfter);
    }
}

std::vector<char> SocketUDP::receive(uint8_t type) {
    std::vector<char> result;
    std::vector<char> datagram;
    PacketHeader packetHeader = {0, 2, 0};
    do {
        datagram = sock.receive(MSG_PEEK);
        if (datagram[9] == type) { // TODO, fix this
            sock.receive();
            Utils::printVector(datagram, "Received datagram");
            auto[header, body] = Utils::divideHeader(sizeof(PacketHeader), datagram);
            packetHeader = Utils::deserializeStruct<PacketHeader>(header);
            result.insert(result.end(), body.begin(), body.end());
            do {
                // send
            } while(!sock.isDataPresent());
        }
    } while(packetHeader.total - packetHeader.current > 1);
    return result;
}


