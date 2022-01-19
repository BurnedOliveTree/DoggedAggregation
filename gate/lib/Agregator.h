#pragma once

#include "Header.h"
#include <vector>
#include <iomanip>
#include <map>


class Agregator{
public:
    std::vector<std::map<uint16_t, std::vector<char>>> docBuilder;
    std::vector<std::map<uint16_t, uint16_t>> timestamps;
    std::vector<std::map<uint16_t, uint16_t>> packetCounter;
    std::vector<std::map<uint16_t, uint8_t>> error;
    Agregator(int nSerwers);
};