#pragma once

#include "allocators/allocator_meta.h"

#include "log.h"

#include <functional>

namespace nostalgia {

    enum class AllocatorFlags : uint32_t {
        NONE = 0,
        FIXED_SIZE              = 1 << 0,
        VARIABLE_SIZE           = 1 << 1,
        THREAD_SAFE             = 1 << 2,

    };
    ENABLE_BITMASK_OPERATORS(AllocatorFlags);

    struct BenchmarkType {

        const std::string label;
		const std::string description;
        const AllocatorFlags compatibleFlags;

        bool disabled = false;

        const std::function<void()> run;

        bool compatible(AllocatorFlags flags) const {
			return (compatibleFlags & flags) == flags;
        }
    };

    struct BenchmarkMetadata {

        // ID or merely label?
        const std::string label;
        const std::string description;
        const allocatorTestFlags testFlags;
        const std::function<void()> run; // Pointer to the benchmark function

        // Flags for eligible benchmarks
        bool has(allocatorTestFlags flag) const {
            return hasFlag(testFlags, flag);
        }
    };
}