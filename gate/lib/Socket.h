#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <arpa/inet.h>



constexpr int MAX_PACKET_SIZE = 256;

class Socket{
public:
    struct sockaddr_in desc_4;
    struct sockaddr_in6 desc_6;
    struct sockaddr* self_addr;
    struct sockaddr dest_addr;
    struct pollfd fds[200];
    bool is_server;
    bool is_datagram;

    socklen_t socket_len;
    socklen_t dest_len;
    std::string socket_ip;
    int socket_port, sock, msgsock, timeout;
    int on = 1, nfds = 1, current_size = 0;


    Socket(std::string ip = "127.0.0.1" , int port = 8000, bool is_serv = false, bool is_UDP = true);
    ~Socket();
    void Bind();
    void Connect();
    void Listen();
    void Write(std::vector<char> msg, int which_socket = 0);
    void Send(std::vector<char> msg);
    std::vector<char> Receive();
    std::vector<char> Read(size_t n_bytes, int which_socket = 0);

};
