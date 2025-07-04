#pragma once

#include "allocators/allocator_meta.h"

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
		const std::string description;
        const AllocatorFlags compatibleFlags;

        bool disabled = false;

        const std::function<void()> dispatcher;

        bool compatible(AllocatorFlags flags) const {
            return (compatibleFlags & flags) == flags;
        };
    };
}