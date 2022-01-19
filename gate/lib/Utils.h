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
namespace Utils{

struct ex_struct{
    uint16_t a;
    uint8_t b;
    uint8_t c;
};   

template<typename T>
std::vector<char> serializeStruct(T s){
    size_t len_st = sizeof(s);
    char* ptr = reinterpret_cast<char*>(&s);
    std::vector<char> result(ptr, ptr+len_st*sizeof(char));
    return result;
}

template<typename T>
T deserializeStruct(std::vector<char> vc){
    T* st = reinterpret_cast<T*>(vc.data());
    return *st;
}

std::vector<char> serializeString(std::string s);
std::string deserializeString(std::vector<char> vc);

template<typename T>
std::vector<char> addHeader(T st, std::vector<char> vc){
    size_t len_st = sizeof(st);
    char* ptr = reinterpret_cast<char*>(&st);
    std::vector<char> result(ptr, ptr+len_st*sizeof(char));
    result.insert(result.end(), vc.begin(), vc.end());
    return result;
}

std::vector<char> addHeader(char* ptr, size_t len_st, std::vector<char> vc);
std::vector<char> addHeader(std::vector<char> st, std::vector<char> vc);

std::pair<std::vector<char>,std::vector<char>> divideHeader(size_t h_len, std::vector<char> vc);
void printVector(std::vector<char> vc);

std::vector<std::vector<char>> splitData(std::vector<char>data, size_t maxsize);
std::map<std::string, std::string> readConfig();

std::vector<char> joinData(std::vector<std::vector<char>>data);

}
