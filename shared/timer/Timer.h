#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:

    Timer();
    void start();
    void stop();
    double getTime() const;
    void resetTime();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool running;
};

#endif 
