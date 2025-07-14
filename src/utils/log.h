#pragma once

#include "utils/bitmask_operators.h"
#include <string>
#include <format>
#include <iostream>
#include <type_traits>

namespace nostalgia {
	enum class LogFlags : uint32_t {
		NONE = 0,
		Info = 1 << 0,
		Warn = 1 << 1,
		Error = 1 << 2,
		Debug = 1 << 3
	};
	ENABLE_BITMASK_OPERATORS(LogFlags);
}
namespace nostalgia::log {
	namespace {
		constexpr std::string_view label_empty = "      ";
		constexpr std::string_view label_debug = "[DEBUG]";
		constexpr std::string_view label_error = "[ERROR]";
		constexpr std::string_view label_warn  = "[WARN] ";
		constexpr std::string_view label_info  = "[INFO] ";
	}

	template <typename... Args>
	inline void print(LogFlags flags, const std::format_string<Args...> fmt, Args&&... args) {
		std::string print_message = std::format(fmt, std::forward<Args>(args)...);

		print_message = std::format("{}{}{} {}", 
			hasFlag(flags, LogFlags::Debug) ? label_debug : (
				hasFlag(flags, LogFlags::Error) ? label_error : label_empty),
			hasFlag(flags, LogFlags::Warn) ? label_warn : label_empty,
			hasFlag(flags, LogFlags::Error) ? label_error : label_empty,
			print_message);

		std::cout << print_message << std::endl; 
	}

	template <typename... Args>
	inline void print(const std::format_string<Args...> fmt, Args&&... args) {
		print(LogFlags::Info, fmt, std::forward<Args>(args)...);
	}
}