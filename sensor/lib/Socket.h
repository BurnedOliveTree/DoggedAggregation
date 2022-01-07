#pragma once

#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <arpa/inet.h>


constexpr int MAX_PACKET_SIZE = 256;

class Socket{
    struct sockaddr_in desc_4;
    struct sockaddr_in6 desc_6;
    struct sockaddr* self_addr;

    socklen_t socket_len;
    std::string socket_ip;
    int socket_port, socket_fd;

public:
    explicit Socket(const std::string& ipAddress, int port);
    ~Socket();
    void send(std::vector<char> message);
    std::vector<char> receive(int flag = 0);
    bool isDataPresent();
};
