#include "Utils.h"

namespace Utils {
    std::vector<char> serializeString(std::string message) {
        return {message.begin(), message.end()};
    }

    std::string deserializeString(std::vector<char> message) {
        return {message.begin(), message.end()};
    }

    std::vector<char> addHeader(std::vector<char> header, std::vector<char> body) {
        std::vector<char> result = header;
        result.insert(result.end(), body.begin(), body.end());
        return result;
    }

    std::pair<std::vector<char>,std::vector<char>> divideHeader(size_t h_len, std::vector<char> data) {
        return std::make_pair(std::vector<char>(data.begin(), data.begin() + h_len), std::vector<char>(data.begin() + h_len, data.end()));
    }

    std::vector<std::vector<char>> splitData(std::vector<char> data, size_t maxSize) {
        size_t dataSize = data.size();
        std::vector<std::vector<char>> splits;
        int number_of_parts = dataSize / maxSize + (dataSize % maxSize != 0);
        for (int part = 0; part < number_of_parts; part++)
            splits.push_back(std::vector<char>(data.begin()+ part * maxSize, std::min(data.begin() + (part + 1) * maxSize, data.end())));
        return splits;
    }

    void printVector(const std::vector<char>& vector){
        std::cout << "[Utils.cpp:40] VecSize: " << vector.size() << "|\t";
        for (char i : vector)
            std::cout << " " << unsigned(i);
        std::cout << std::endl;
    }
}