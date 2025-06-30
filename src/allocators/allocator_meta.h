#pragma once

#include <string>

#include "log.h"

namespace nostalgia {
    enum class allocatorTestFlags : uint32_t {
        NONE = 0,
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