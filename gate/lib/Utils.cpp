#include "Utils.h"

namespace Utils{

std::vector<char> serializeString(std::string s){
    return std::vector<char>(s.begin(), s.end());
}

std::string deserializeString(std::vector<char> vc){
    return std::string(vc.begin(), vc.end());
}

std::vector<char> addHeader(char* ptr, size_t len_st, std::vector<char> vc){
    std::vector<char> result(ptr, ptr+len_st*sizeof(char));
    result.insert(result.end(), vc.begin(), vc.end());
    return result;
}

std::vector<char> addHeader(std::vector<char> st, std::vector<char> vc){
    std::vector<char> result = st;
    result.insert(result.end(), vc.begin(), vc.end());
    return result;
}

std::pair<std::vector<char>,std::vector<char>> divideHeader(size_t h_len, std::vector<char> vc){
    return std::make_pair(std::vector<char>(vc.begin(), vc.begin()+h_len),std::vector<char>(vc.begin()+h_len, vc.end()));
}

std::vector<std::vector<char>> splitData(std::vector<char>data, size_t maxsize){
    size_t data_size = data.size();
    std::vector<std::vector<char>> splits;
    int number_of_parts = data_size/maxsize+(data_size%maxsize!=0);
    for(int part = 0; part<number_of_parts; part++){
        splits.push_back(std::vector<char>(data.begin()+part*maxsize, std::min(data.begin()+(part+1)*maxsize, data.end())));
    }
    return splits;
}

void printVector(std::vector<char> vc){
    std::cout << "VecSize: " << vc.size()<<"|\t";
    for(auto i = vc.begin(); i!= vc.end(); i++){
        std::cout << " " << unsigned(*i); 
    }
    std::cout << std::endl;
}

std::map<std::string, std::string> readConfig() {
    std::ifstream config("../gate.config");
    std::string line;
    auto result = std::map<std::string, std::string>();
    while (std::getline(config, line)) {
        auto signIndex = line.find('=');
        result[line.substr(0, signIndex)] = line.substr(signIndex + 1, line.size());
    }
    config.close();
    return result;
}

}