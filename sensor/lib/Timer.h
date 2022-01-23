#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <random>

#include "Utils.h"


class Timer {
public:
    std::chrono::nanoseconds tick;

    static Timer& getInstance();
    uint16_t getCounter() const;
    void setCounter(uint16_t value);

    Timer(Timer const&) = delete;
    void operator=(Timer const&) = delete;
private:
    uint16_t counter;
    std::atomic<bool> isCounting;
    std::thread internalThread;

    Timer();
    ~Timer();
    void increment();
};