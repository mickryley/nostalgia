#pragma once

#include "utils/log.h"

namespace nostalgia {
    enum class AllocatorID {
        Linear,
        Stack,
        Pool,
        Freelist
    };

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

    struct AllocatorType {
        const AllocatorID id;
        const std::string label;
        const std::string description;
        const AllocatorFlags required_flags;

        bool has(AllocatorFlags flag) const {
            return hasFlag(required_flags, flag);
        }
        bool is_compatible_with(AllocatorFlags compatible_flags) const {
            return (compatible_flags == AllocatorFlags::NONE || ((compatible_flags & required_flags) == required_flags));
		}
    };
}