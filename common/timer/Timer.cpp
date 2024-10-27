#include "Timer.hpp"

Timer::Timer() : running_(false), startTime_(), endTime_(){}

void Timer::start() {
    startTime_ = std::chrono::high_resolution_clock::now();
    running_ = true;
}

void Timer::stop() {
    if (running_) {
        endTime_ = std::chrono::high_resolution_clock::now();
        running_ = false;
    }
}

double Timer::getTime() const {
    if (running_) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(currentTime - startTime_).count();
    } else {
        return std::chrono::duration<double, std::milli>(endTime_ - startTime_).count();
    }
}

void Timer::reset() {
    startTime_ = std::chrono::high_resolution_clock::now();
    endTime_ = startTime_;
    running_ = false;
}

