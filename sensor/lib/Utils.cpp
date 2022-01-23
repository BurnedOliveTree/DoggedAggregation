#include "Utils.h"

namespace Utils {
    std::vector<char> serializeString(std::string message) {
        return {message.begin(), message.end()};
    }

    std::string deserializeString(std::vector<char> message) {
        return {message.begin(), message.end()};
    }

    std::vector<char> serializeTime(uint16_t time) {
        std::vector<char> result;
        std::memcpy(result.data(), &time, sizeof(time));
        return result;
    }

    uint16_t deserializeTime(std::vector<char> time) {
        uint16_t result;
        std::memcpy(&result, time.data(), sizeof(result));
        return result;
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

    void printVector(const std::vector<char>& vector, std::string additional) {
        std::cout << "[Utils.cpp:44] " << additional << "; size: " << vector.size() << ", content:";
        for (char i : vector)
            std::cout << " " << unsigned(i);
        std::cout << std::endl;
    }

    std::map<std::string, std::string> readConfig() {
        std::ifstream config("../sensor.config");
        std::string line;
        auto result = std::map<std::string, std::string>();
        while (std::getline(config, line)) {
            auto signIndex = line.find('=');
            result[line.substr(0, signIndex)] = line.substr(signIndex + 1, line.size());
        }
        config.close();
        return result;
    }

    std::chrono::nanoseconds ping() {
        char buffer[256];
        auto file = popen("ping 127.0.0.1 -c 1", "r");
        fgets(buffer, 256, file);
        
        fgets(buffer, 256, file);
        pclose(file);

        std::smatch result[3];
        auto temp = std::string(buffer);
        std::regex_search(temp, result[0], std::regex("time=.*"));
        temp = std::string(result[0][0]);
        std::regex_search(temp, result[1], std::regex("[0-9]+\\.[0-9]+"));
        std::regex_search(temp, result[2], std::regex("[mn]?s"));

        double value;
        if (result[2][0] == "ms") {
            value = 1000000 * stod(result[1][0]);
        } else if(result[2][0] == "s") {
            value = stod(result[1][0]);
        } else {
            throw std::invalid_argument("Ping returned a value in other units than: ms, s!");
        }
        return std::chrono::nanoseconds(static_cast<long long>(value));
    }
}