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
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> _endTime;
    bool _running;
};

#endif TIMER_HPP

