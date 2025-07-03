#pragma once

#include "log.h"

namespace nostalgia {

    // Vector of all allocators is held for now in the benchmarking_manager

    enum class AllocatorFlags : uint32_t {
        NONE = 0,
        FIXED_SIZE              = 1 << 0,
        VARIABLE_SIZE           = 1 << 1,
        THREAD_SAFE             = 1 << 2,

    };
    ENABLE_BITMASK_OPERATORS(AllocatorFlags);

    enum class AllocatorID {
        Linear,
        Stack,
        Pool,
        Freelist
    };

    struct AllocatorType {

        const AllocatorID id;
        const std::string label;
        const std::string description;
        const AllocatorFlags requiredFlags;

        bool has(AllocatorFlags flag) const {
            return hasFlag(requiredFlags, flag);
        }
    };
}