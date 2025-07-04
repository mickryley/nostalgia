#pragma once

#include "log.h"

namespace nostalgia {

    // Vector of all allocators is held for now in the benchmarking_manager

    enum class AllocatorFlags : uint32_t {
        NONE = 0,
        FIXED_SIZE              = 1 << 0,
        VARIABLE_SIZE           = 1 << 1,
        THREAD_SAFE             = 1 << 2,
		DEALLOC_REWIND          = 1 << 3,
		DEALLOC_REVERSE         = 1 << 4,
		DEALLOC_FORWARD         = 1 << 5,
		DEALLOC_RANDOM          = 1 << 6,

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
        bool isCompatibleWith(AllocatorFlags compatibleFlags) const {
            return (compatibleFlags == AllocatorFlags::NONE || ((compatibleFlags & requiredFlags) == requiredFlags));
		}

    };
}