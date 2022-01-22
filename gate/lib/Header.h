#pragma once
#include <stdint.h>
#include <string>

struct DocumentHeader {
    uint16_t documentId;
    uint8_t documentType;
    uint8_t type;
};

struct PHeader{
    uint16_t timestamp;
    uint16_t total;
    uint16_t current;
};

struct AgregatedHeader {
    uint16_t documentId;
    uint8_t documentType;
    uint8_t statusCode;
    uint8_t hash;
};

struct THeader{
    uint16_t total;
    uint16_t current;
    uint32_t size;
};