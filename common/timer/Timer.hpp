#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:

    Timer();
    void start();
    void stop();
    double getTime() const;
    void reset();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime_;
    bool running_;
};

#endif TIMER_HPP

