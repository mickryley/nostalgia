#pragma once

#include "allocators/allocator_meta.h"

#include "log.h"

#include <string>
#include <functional>

namespace nostalgia {
// Vector of all benchmarks is held for now in the benchmarking_manager
    struct BenchmarkType {

        const std::string label;
		const std::string description;
        const AllocatorFlags compatibleFlags;

        bool disabled = false;

        const std::function<void()> dispatcher;

        bool compatible(AllocatorFlags flags) const {
			return (compatibleFlags & flags) == flags;
        }
    };

    struct ImplementationType {
        const std::string label;
		const std::string description;
        const AllocatorFlags compatibleFlags;

        const std::function<void()> run;
        bool compatible(AllocatorFlags flags) const {
			return (compatibleFlags & flags) == flags;
        }
    };
}