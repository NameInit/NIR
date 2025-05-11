#pragma once
#include <chrono>

class Timer{
	private:
		std::chrono::high_resolution_clock::time_point __start_time;
		std::chrono::high_resolution_clock::time_point __end_time;
		std::chrono::high_resolution_clock::time_point __start_time_sleep;
		std::chrono::high_resolution_clock::time_point __end_time_sleep;
		double __offset=0.;
	public:
		void refresh_offset() { __offset=0.; } 
		void sleep() { __start_time_sleep=std::chrono::high_resolution_clock::now(); }
		void unsleep() { __end_time_sleep=std::chrono::high_resolution_clock::now(); __offset += std::chrono::duration_cast<std::chrono::milliseconds>(__end_time - __start_time).count()/1000.; }
		void start() { __start_time=std::chrono::high_resolution_clock::now(); }
		void end() { __end_time=std::chrono::high_resolution_clock::now(); }
		double duration_s() { return (std::chrono::duration_cast<std::chrono::milliseconds>(__end_time - __start_time).count()-__offset)/1000.; }
};