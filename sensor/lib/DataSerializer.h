#pragma once
#include <variant>

#include "SocketInterface.h"

struct StandardHeader {
    uint16_t documentId;
    uint8_t documentType;
    uint8_t type;
};

struct TimeHeader {
    uint8_t type;
};

class DataSerializer{
    static std::string handleString(std::vector<char> data);
    static uint16_t handleTime(std::vector<char> data);
public:
    explicit DataSerializer();
    static std::vector<char> serialize(std::string message, uint16_t documentId, uint8_t documentType);
    static std::vector<char> serialize(uint16_t time, uint16_t documentId, uint8_t documentType);
    static std::variant<std::string, uint16_t> deserialize(std::vector<char> data);
    static void printVariant(std::variant<std::string, uint16_t> msg);
};