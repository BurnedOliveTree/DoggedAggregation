#include "Timer.h"

Timer &Timer::getInstance() {
    static Timer instance;
    return instance;
}

uint16_t Timer::getCounter() const {
    return counter;
}

void Timer::setCounter(uint16_t value) {
    counter = value;
}

void Timer::increment() {
    while (isCounting) {
        std::this_thread::sleep_for(tick);
        counter++;
    }
}

Timer::Timer() {
    counter = 0;
    isCounting = true;
    tick = std::chrono::nanoseconds(std::stol(Utils::readConfig()["tickDuration"]));
    internalThread = std::thread(&Timer::increment, this);
}

Timer::~Timer() {
    isCounting = false;
}
