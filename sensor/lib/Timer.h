#pragma once

#include <thread>

constexpr auto TICK = std::chrono::nanoseconds(5000000000);

class Timer {
public:
    static Timer& getInstance();
    uint16_t getCounter() const;
    void setCounter(uint16_t value);

    Timer(Timer const&) = delete;
    void operator=(Timer const&) = delete;
private:
    uint16_t counter;
    bool isCounting;
    std::thread internalThread;

    Timer();
    ~Timer();
    void increment();
};