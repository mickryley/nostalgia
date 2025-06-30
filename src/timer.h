#pragma once

#include "log.h"

#include <chrono>

namespace nostalgia::timer {

	// High Speed Timer Wrapper 
	class Timer {
	public:
		// Constructor
		Timer(const char* label) : label(label) {
			log::print("Timer '{}' created.", label);
		}
		// Destructor
		~Timer() {
			log::print("Timer '{}' destroyed.", label);
		}
		// Start the timer
		void start() {
			start_time = std::chrono::high_resolution_clock::now();
		}
		// Stop the timer and return elapsed time in milliseconds
		double stop() {
			auto end_time = std::chrono::high_resolution_clock::now();
			total_time += std::chrono::duration<double, std::milli>(end_time - start_time).count();
			log::print("Timer stopped. Elapsed time: {} ms", total_time);
			return total_time;
		}
		void pause() {
			auto pause_time = std::chrono::high_resolution_clock::now();
			total_time += std::chrono::duration<double, std::milli>(pause_time - start_time).count();
			//log::print("Timer paused. Total time: {} ms", total_time);
		}
		double print() const {
			log::print("Timer '{}' total time: {} ms", label, total_time);
			return total_time;
		}
	private:
		const char* label;
		std::chrono::high_resolution_clock::time_point start_time;
		double total_time = 0.0;
	};
}