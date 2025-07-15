#pragma once

#define ENABLE_BITMASK_OPERATORS(x) \
    inline x operator|(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) | static_cast<std::underlying_type_t<x>>(b)); } \
    inline x operator&(x a, x b) { return static_cast<x>(static_cast<std::underlying_type_t<x>>(a) & static_cast<std::underlying_type_t<x>>(b)); } \
    inline x& operator|=(x& a, x b) { a = a | b; return a; } \
    inline x operator~(x a) { return static_cast<x>(~static_cast<std::underlying_type_t<x>>(a)); } \
    inline bool hasFlag(x value, x flag) { return (value & flag) != x::NONE; } \
    inline bool hasAllRequiredFlags(x value, x requiredFlags) { return (requiredFlags == x::NONE) || ((value & requiredFlags) == requiredFlags); } \
    inline bool hasOnlyCompatibleFlags(x value, x compatibleFlags) { return (compatibleFlags == x::NONE) || ((value & ~compatibleFlags) == x::NONE); }
