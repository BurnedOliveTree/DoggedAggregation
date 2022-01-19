#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace Utils {
    template<typename T>
    std::vector<char> serializeStruct(T s) {
        size_t len_st = sizeof(s);
        char* ptr = reinterpret_cast<char*>(&s);
        std::vector<char> result(ptr, ptr+len_st*sizeof(char));
        return result;
    }
    template<typename T>
    T deserializeStruct(std::vector<char> vc) {
        T* st = reinterpret_cast<T*>(vc.data());
        return *st;
    }
    std::vector<char> serializeString(std::string message);
    std::string deserializeString(std::vector<char> message);
    std::vector<char> serializeTime(uint16_t time);
    uint16_t deserializeTime(std::vector<char> time);

    template<typename T>
    std::vector<char> addHeader(T st, std::vector<char> vc) {
        size_t len_st = sizeof(st);
        char* ptr = reinterpret_cast<char*>(&st);
        std::vector<char> result(ptr, ptr+len_st*sizeof(char));
        result.insert(result.end(), vc.begin(), vc.end());
        return result;
    }
    std::vector<char> addHeader(std::vector<char> header, std::vector<char> body);
    std::pair<std::vector<char>, std::vector<char>> divideHeader(size_t h_len, std::vector<char> data);
    void printVector(const std::vector<char>& vector, std::string additional = "");
    std::vector<std::vector<char>> splitData(std::vector<char>data, size_t maxSize);
    std::map<std::string, std::string> readConfig();
    std::chrono::nanoseconds ping();
}
