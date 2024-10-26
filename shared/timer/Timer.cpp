#include "Timer.h"

Timer::Timer() : running(false) {}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    running = true;
}

void Timer::stop() {
    if (running) {
        endTime = std::chrono::high_resolution_clock::now();
        running = false;
    }
}

double Timer::getTime() const {
    if (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(currentTime - startTime).count();
    } else {
        return std::chrono::duration<double, std::milli>(endTime - startTime).count();
    }
}

void Timer::resetTime() {
    startTime = std::chrono::high_resolution_clock::now();
    endTime = startTime;
    running = false;
}
