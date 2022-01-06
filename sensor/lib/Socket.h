#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
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
    explicit Socket(const std::string& ipAddress = "127.0.0.1" , int port = 8000);
    ~Socket();
    void send(std::vector<char> message);
    std::vector<char> receive();
};
