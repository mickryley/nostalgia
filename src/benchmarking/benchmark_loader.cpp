#include "benchmarking/benchmark_loader.h"
#include "benchmarking/benchmark_atlas.h"

#include "ui/gui.h"

namespace nostalgia::benchmarking::loader {

    void loadBenchmark(const nostalgia::BenchmarkID id) {
        //nostalgia::BenchmarkType benchmarkType = nostalgia::benchmark::atlas[id];
        const auto& benchmarkType = nostalgia::benchmark::atlas.at(id);
        if (!benchmarkType.paramSpecs.empty()) {
            nostalgia::gui::load_benchmarking_params(benchmarkType.paramSpecs, m_paramOutput);
        } 
    }

      void dispatchBenchmark(const nostalgia::BenchmarkID id) {
        const auto& benchmarkType = nostalgia::benchmark::atlas.at(id);
        if (benchmarkType.dispatcher) {
            benchmarkType.dispatcher();
        }
    }
}