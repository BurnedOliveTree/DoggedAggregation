#include "Socket.h"
#include "Utils.h"
#include <iostream>

Socket::Socket(std::string ip, int port, bool is_serv,bool is_UDP){
    is_server = is_serv;
    is_datagram = is_UDP;
    socket_ip = ip;
    socket_port = port;
    if (std::string(ip).find('.') != std::string::npos){
        desc_4.sin_family = AF_INET;
        desc_4.sin_port = htons( socket_port );

        if( inet_pton( AF_INET, socket_ip.c_str(), & desc_4.sin_addr ) <= 0 )
        {
            throw std::runtime_error("inet_pton didn't convert IP");
        }
        if(is_UDP){
            sock = socket( AF_INET, SOCK_DGRAM, 0 );
        } else {
            sock = socket( AF_INET, SOCK_STREAM, 0 );
        }
        if(( sock ) < 0 )
        {
            throw std::runtime_error("Socket wasn't created");
        }
        socket_len = sizeof(desc_4);
        self_addr = (struct sockaddr*) &desc_4;
    }
    else {
        desc_6.sin6_family = AF_INET6;
        desc_6.sin6_port = htons( socket_port );

        if( inet_pton( AF_INET6, socket_ip.c_str(), & desc_6.sin6_addr ) <= 0 )
        {
            throw std::runtime_error("inet_pton didn't convert IP");
        }
    
        if(is_UDP){
            sock = socket( AF_INET, SOCK_DGRAM, 0 );
        } else {
            sock = socket( AF_INET, SOCK_STREAM, 0 );
        }
        if(( sock ) < 0 )
        {
            throw std::runtime_error("Socket wasn't created");
        }
        socket_len = sizeof(desc_6);
        self_addr = (struct sockaddr*) &desc_6;
    }
}

Socket::~Socket(){
    for (int i = 0; i < nfds; i++)
    {
        if(fds[i].fd >= 0)
        close(fds[i].fd);
    }
}

void Socket::Bind(){
    if( bind( sock, self_addr, socket_len ) < 0 )
    {
        throw std::runtime_error("Socket wasn't binded");
    }
}

void Socket::Connect(){
    struct sockaddr* dst;
    socklen_t dst_len;
    if(is_server){
        dst = &dest_addr;
        dst_len = dest_len;
    }
    else{
        dst = self_addr;
        dst_len = socket_len;
    }
    if (connect(sock, dst, dst_len) < 0) {
        throw std::runtime_error("Couldn't connect to server");
    }
}

void Socket::Listen(){
    listen(sock, 2);
    msgsock = accept(sock,(struct sockaddr *) 0,(socklen_t *) 0); 
}

std::vector<char> Socket::Read(size_t n_bytes){
    std::vector<char> buffer(MAX_PACKET_SIZE);
    int rval = 0, rall = 0;
    if (msgsock < 0) {
        throw std::runtime_error("Couldn't accept connection");
    }
    do {
        if ((rval = read(msgsock,buffer.data()+rall, n_bytes-rall)) == -1) {
            throw std::runtime_error("Error while reading stream");
        }
        rall += rval;

    } while (n_bytes-rall>0);
    buffer.resize(rall);
    return buffer;
}

void Socket::Write(std::vector<char> msg){
    struct sockaddr* dst;
    socklen_t dst_len;
    int sall = 0, sval = 0;
    int bsize = msg.size();
    std::cout<< "TCP msg:\n";
    Utils::printVector(msg);
    do{
        if((sval = send(msgsock, msg.data()+sall, bsize-sall, 0)) < 0){  // not sure if sock or msgsock
            throw std::runtime_error("Couldn't write message to stream");
        }
        sall += sval;
    } while(bsize-sall>0);
}

void Socket::Send(std::vector<char> msg){
    struct sockaddr* dst;
    socklen_t dst_len;
    if(is_server){
        dst = &dest_addr;
        dst_len = dest_len;
    }
    else{
        dst = self_addr;
        dst_len = socket_len;
    }
    int bsize = msg.size();
    int er;
    er = sendto(sock, msg.data(), bsize, 0, self_addr, socket_len );
    if(er<0)
        {
            throw std::runtime_error("Couldn't send message to server " + std::to_string(er));
        }
}

std::vector<char> Socket::Receive(){
    std::vector<char> buffer(MAX_PACKET_SIZE);
    struct sockaddr* dst;
    socklen_t* dst_len;
    if(is_server){
        dst = &dest_addr;
        dst_len = &dest_len;
    }
    else{
        dst = self_addr;
        dst_len = &socket_len;
    }
    int result = recvfrom(sock, buffer.data(), buffer.size(), 0, self_addr, &socket_len);
    if( result < 0 )
        {   
            throw std::runtime_error("Couldn't receive message from server");
        }
    buffer.resize(result);
    return buffer;
}
