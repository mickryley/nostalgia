#pragma once

#include "allocators/info/allocator_meta.h"
#include "benchmark_params.h"

#include "utils/log.h"

#include <string>
#include <functional>

namespace nostalgia {

    enum class BenchmarkID {
        NONE,
        IBM_Bursts
    };

    struct benchmark_type {
        const std::string label;
        const std::string label_long;
		const std::string description;
        const AllocatorFlags required_flags;

        bool disabled = false;

        const std::function<void()> dispatcher;
        const std::function<void()> loader; // Unused for now - remove
        const std::vector<BenchmarkParamSpec> paramSpecs;
    };
}