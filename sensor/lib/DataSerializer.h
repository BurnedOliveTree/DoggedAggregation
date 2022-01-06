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
    static SimpleStruct handleStruct(std::vector<char> data);
    static std::string handleString(std::vector<char> data);
public:
    explicit DataSerializer();
    static std::vector<char> serialize(std::string message);
    static std::vector<char> serialize(SimpleStruct message);
    static std::variant<std::string, SimpleStruct> deserialize(std::vector<char> data);
    static void printVariant(std::variant<std::string, SimpleStruct> msg);
};