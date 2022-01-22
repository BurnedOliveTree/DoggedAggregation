#pragma once
#include <variant>

#include "SocketInterface.h"

class DataSerializer{
    static std::string handleString(std::vector<char> data);
    static uint16_t handleTime(std::vector<char> data);
public:
    explicit DataSerializer();
    static std::pair<StandardHeader, std::vector<char>> serialize(std::string message, uint16_t documentId, uint8_t documentType);
    static std::pair<StandardHeader, std::vector<char>> serialize(uint16_t time, uint16_t documentId, uint8_t documentType);
    static std::variant<std::string, uint16_t> deserialize(std::vector<char> data);
    static void printVariant(std::variant<std::string, uint16_t> msg);
};