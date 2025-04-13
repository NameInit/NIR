#pragma once
#include <chrono>

class Timer{
    private:
        std::chrono::high_resolution_clock::time_point __start_time;
        std::chrono::high_resolution_clock::time_point __end_time;
        double __offset;
    public:
        void set_start() { __start_time=std::chrono::high_resolution_clock::now(); }
        void set_end() { __end_time=std::chrono::high_resolution_clock::now(); }
        void set_offset(double offset) { __offset=offset; }
        double duration_s() { return (std::chrono::duration_cast<std::chrono::milliseconds>(__end_time - __start_time).count()-__offset)/1000.; }
};