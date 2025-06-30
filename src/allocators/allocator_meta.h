#pragma once

#include <string>

#define ENABLE_BITMASK_OPERATORS(x) \
inline x operator|(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) | static_cast<std::underlying_type_t<x>>(b)); } \
inline x operator&(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) & static_cast<std::underlying_type_t<x>>(b)); } \
inline x& operator|=(x& a, x b) { a = a | b; return a; } \
inline bool hasFlag(x value, x flag) { return (value & flag) != x::None; }

namespace nostalgia {
    enum class allocatorTestFlags : uint32_t {
        None = 0,
        FIXED_SIZE = 1 << 0,
        THREAD_SAFE = 1 << 1,
    };
    ENABLE_BITMASK_OPERATORS(allocatorTestFlags);

    struct allocatorMetadata {

        // ID or merely label?
        const std::string label;
        const std::string description;
        const allocatorTestFlags testFlags;

        // Flags for eligible benchmarks
        bool has(allocatorTestFlags flag) const {
            return hasFlag(testFlags, flag);
        }
    };
}