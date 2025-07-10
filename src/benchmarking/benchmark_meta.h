#pragma once

#include "allocators/allocator_meta.h"
#include "benchmark_params.h"

#include "log.h"

#include <string>
#include <functional>

namespace nostalgia {

    enum class BenchmarkID {
        NONE,
        IBM_Bursts
    };

    struct BenchmarkType {
        const std::string label;
        const std::string labelLong;
		const std::string description;
        const AllocatorFlags compatibleFlags;

        bool disabled = false;

        const std::function<void()> dispatcher;
        const std::function<void()> loader; // Unused for now - remove
        const std::vector<BenchmarkParamSpec> paramSpecs;

        bool compatible(AllocatorFlags flags) const {
            return (compatibleFlags & flags) == flags;
        };
    };
}