#pragma once

#include <string>
#include <format>
#include <iostream>
#include <type_traits>

#define ENABLE_BITMASK_OPERATORS(x) \
inline x operator|(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) | static_cast<std::underlying_type_t<x>>(b)); } \
inline x operator&(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) & static_cast<std::underlying_type_t<x>>(b)); } \
inline x& operator|=(x& a, x b) { a = a | b; return a; } \
inline bool hasFlag(x value, x flag) { return (value & flag) != x::NONE; }

namespace nostalgia {
	enum class logFlags : uint32_t {
		NONE = 0,
		INFO = 1 << 0,
		WARN = 1 << 1,
		ERROR = 1 << 2,
		DEBUG = 1 << 3
	};
	ENABLE_BITMASK_OPERATORS(logFlags);
}
namespace nostalgia::log {

	template <typename... Args>
	inline void print(const std::format_string<Args...> fmt, Args&&... args) {

		std::string formatted = std::format(fmt, std::forward<decltype(args)>(args)...);
		std::cout << "[INFO] " << formatted << std::endl;
	}

	template <typename... Args>
	inline void print(logFlags flags, const std::format_string<Args...> fmt, Args&&... args) {
		
		std::string formatted = std::format(fmt, std::forward<decltype(args)>(args)...);
		if (hasFlag(flags, logFlags::INFO)) {
			formatted = "[INFO] " + formatted;
		} 
		if (hasFlag(flags, logFlags::WARN)) {
			formatted = "[WARN] " + formatted;
		} 
		if (hasFlag(flags, logFlags::ERROR)) {
			formatted = "[ERROR] " + formatted;
		} 
		if (hasFlag(flags, logFlags::DEBUG)) {
			formatted = "[DEBUG] " + formatted;
		}
		std::cout << formatted << std::endl; 
	}
}