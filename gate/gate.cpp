#include "lib/Gate.h"
#include "lib/Timer.h"
#include <iostream>
#include <array>
#include <atomic>
#include <memory>
#include <variant>
#include <thread>

int main() {
    std::atomic<bool> isProgramRunning = true;
    auto gate = Gate();
    auto timer = &Timer::getInstance();
    std::variant<std::string, SimpleStruct> msg;
    
    while (isProgramRunning) {
        msg = gate.sensorGate->Receive(true);
        if(std::get_if<std::string>(&msg)){ 
            std::cout << "Received string message: " << std::get<std::string>(msg) << std::endl;
        }
        if(std::get_if<SimpleStruct>(&msg)){ 
            std::cout << "Received struct message: " << std::get<SimpleStruct>(msg).a << " " << unsigned(std::get<SimpleStruct>(msg).b) << " " << unsigned(std::get<SimpleStruct>(msg).c) << std::endl;
        }
    }
}