#pragma once

#include "utils/log.h"

namespace nostalgia {
    enum class AllocatorID {
        NONE = 0,
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
        DEFAULT_MALLOC          = 1 << 7, 

    };
    ENABLE_BITMASK_OPERATORS(AllocatorFlags);

    struct AllocatorType {
        const AllocatorID id;
        const std::string label;
        const std::string description;
        const AllocatorFlags compatible_flags;
        const AllocatorFlags required_flags;

        bool has(AllocatorFlags flag) const {
            return hasFlag(compatible_flags, flag);
        }

        bool is_compatible_with_benchmark(AllocatorFlags flags) const {
            // All flags must be compatible
            return flags == AllocatorFlags::NONE ||
        (flags & ~compatible_flags) == AllocatorFlags::NONE;
    }

        // Need to unify these into one location
        bool is_compatible_with_implementation(AllocatorFlags flags) const {
            // All required flags must be present
            bool passes_required = (required_flags == AllocatorFlags::NONE ||
                 ((flags & required_flags) == required_flags));
            // All flags must be compatible
            bool passes_compatible = (flags == AllocatorFlags::NONE) ||
        ((flags & ~compatible_flags) == AllocatorFlags::NONE);
            return (passes_required && passes_compatible);
		}
    };
}