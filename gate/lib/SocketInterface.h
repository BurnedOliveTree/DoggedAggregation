#pragma once
#include "Socket.h"
#include "Utils.h"
#include "Header.h"
#include <map>
#include <utility>

class SocketInterface{
public:
    virtual void Send(std::vector<char> msg) = 0;
    virtual std::vector<char> Receive(bool echo = false) = 0;
    virtual std::vector<char> ReceiveRaw(bool echo = false) = 0;
};