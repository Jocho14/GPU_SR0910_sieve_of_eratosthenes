#include "Timer.hpp"

Timer::Timer() : _running(false), _startTime(), _endTime(){}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
    running = true;
}

void Timer::stop() {
    if (_running) {
        _endTime = std::chrono::high_resolution_clock::now();
        _running = false;
    }
}

double Timer::getTime() const {
    if (_running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(currentTime - _startTime).count();
    } else {
        return std::chrono::duration<double, std::milli>(_endTime - _startTime).count();
    }
}

void Timer::reset() {
    _startTime = std::chrono::high_resolution_clock::now();
    _endTime = _startTime;
    _running = false;
}

