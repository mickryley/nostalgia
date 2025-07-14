#pragma once
#include "utils/log.h"
#include <chrono>
#include <string>

namespace nostalgia::timer {

	class Timer {
	public:
		Timer(std::string timerLabel) : label(timerLabel) {
			log::print("Timer '{}' created.", label);
		}
		~Timer() {
			log::print("Timer '{}' destroyed.", label);
		}

		void start() {
			start_time = std::chrono::high_resolution_clock::now();
		}
		
		double stop() {
			auto end_time = std::chrono::high_resolution_clock::now();
			total_time += std::chrono::duration<double, std::milli>(end_time - start_time).count();
			return total_time;
		}

		void pause() {
			auto pause_time = std::chrono::high_resolution_clock::now();
			total_time 
			+= std::chrono::duration<double, std::milli>(pause_time - start_time).count();
		}

		double print() const {
			log::print("Timer '{}' total time: {} ms", label, total_time);
			return total_time;
		}

		double gettotal_time() const {
			return total_time;
		}

	private:
		std::string label;
		std::chrono::high_resolution_clock::time_point start_time;
		double total_time = 0.0;
	};
}