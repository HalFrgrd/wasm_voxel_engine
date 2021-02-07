#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class SimpleTimer {
public:
    std::chrono::steady_clock::time_point start_time;
    SimpleTimer() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    int end() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time-start_time);
        
        return duration.count();
    }
};

#endif
