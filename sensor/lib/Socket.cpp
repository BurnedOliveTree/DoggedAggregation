#include "Socket.h"

Socket::Socket(const std::string& ipAddress, int port) {
    socket_ip = ipAddress;
    socket_port = port;
    if (std::string(ipAddress).find('.') != std::string::npos){
        desc_4.sin_family = AF_INET;
        desc_4.sin_port = htons( socket_port );
        if (inet_pton(AF_INET, socket_ip.c_str(), & desc_4.sin_addr ) <= 0 )
            throw std::runtime_error("inet_pton didn't convert IP");
        socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if ((socket_fd) < 0)
            throw std::runtime_error("Socket wasn't created");
        socket_len = sizeof(desc_4);
        self_addr = reinterpret_cast<struct sockaddr*>(&desc_4);
    }
    else {
        desc_6.sin6_family = AF_INET6;
        desc_6.sin6_port = htons( socket_port );
        if (inet_pton(AF_INET6, socket_ip.c_str(), & desc_6.sin6_addr ) <= 0)
            throw std::runtime_error("inet_pton didn't convert IP");
        socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
        if ((socket_fd) < 0)
            throw std::runtime_error("Socket wasn't created");
        socket_len = sizeof(desc_6);
        self_addr = reinterpret_cast<struct sockaddr*>(&desc_6);
    }
    std::cout << "[Socket.cpp:28] Initialized socket properly" << std::endl;
}

Socket::~Socket(){
    shutdown(socket_fd, SHUT_RDWR);
}

std::vector<char> Socket::receive(int flag) {
    std::vector<char> buffer(MAX_PACKET_SIZE);
    ssize_t result = recvfrom(socket_fd, buffer.data(), buffer.size(), flag, self_addr, &socket_len);
    if (result < 0)
        throw std::runtime_error("Couldn't deserialize message from server");
    if (flag != MSG_PEEK)
        std::cout << "[Socket.cpp:40] Received message from server" << std::endl;
    buffer.resize(result);
    return buffer;
}

void Socket::send(std::vector<char> message) {
    if (sendto(socket_fd, message.data(), message.size(), 0, self_addr, socket_len) < 0)
        throw std::runtime_error("Couldn't serialize message to server");
    std::cout << "[Socket.cpp:48] Sent message to server" << std::endl;
}

bool Socket::isDataPresent() {
    std::vector<char> buffer(MAX_PACKET_SIZE);
    return recvfrom(socket_fd, buffer.data(), buffer.size(), MSG_PEEK | MSG_DONTWAIT, self_addr, &socket_len) >= 0;
}
