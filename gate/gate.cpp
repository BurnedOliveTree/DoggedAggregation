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
    std::vector<char> msg;
    
    while (isProgramRunning) {
        msg = gate.sensorGate->ReceiveRaw(true);
        Utils::printVector(msg);
    }
}