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
        std::this_thread::sleep_for(TICK);
        counter++;
    }
}

Timer::Timer() {
    counter = 0;
    isCounting = true;
    internalThread = std::thread(&Timer::increment, this);
}

Timer::~Timer() {
    isCounting = false;
}
