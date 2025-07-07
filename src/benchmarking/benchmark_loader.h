#pragma once

#include "benchmarking/benchmark_meta.h"

namespace nostalgia::benchmarking::loader {

    namespace {
        nostalgia::BenchmarkParams m_paramOutput;
    }

    nostalgia::BenchmarkParams& getParameters();

    void loadBenchmark(const nostalgia::BenchmarkID id);

    void dispatchBenchmark(const nostalgia::BenchmarkID id);
}