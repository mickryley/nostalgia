#pragma once

#include "allocators/allocator_meta.h"

#include "log.h"

#include <functional>

namespace nostalgia {

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