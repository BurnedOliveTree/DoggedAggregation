#pragma once
#include <variant>

#include "SocketInterface.h"

struct TypeHeader{
    uint8_t type;
};   

struct SimpleStruct{
    uint16_t a;
    uint8_t b;
    uint8_t c;
};

class DataSerializer{
    SocketInterface* socketInterface;
public:
    explicit DataSerializer(SocketInterface* _si);

    void send(std::string msg);
    void send(SimpleStruct msg);

    std::variant<std::string, SimpleStruct> receive();
    static SimpleStruct handleStruct(std::vector<char> data);
    static std::string handleString(std::vector<char> data);
};