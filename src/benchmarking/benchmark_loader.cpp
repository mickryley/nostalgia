#include "benchmarking/benchmark_loader.h"
#include "benchmarking/benchmark_atlas.h"

#include "ui/gui.h"

#include "log.h"

namespace nostalgia::benchmarking::loader {

    nostalgia::BenchmarkParams& getParameters() {
        return m_paramOutput;
    }

    void loadBenchmark(const nostalgia::BenchmarkID id) {
        //nostalgia::BenchmarkType benchmarkType = nostalgia::benchmark::atlas[id];
        log::print("Loading benchmark...");
        const auto& benchmarkType = nostalgia::benchmark::atlas.at(id);
        if (!benchmarkType.paramSpecs.empty()) {
            nostalgia::gui::load_benchmarking_params(benchmarkType.paramSpecs);
        } 
    }

      void dispatchBenchmark(const nostalgia::BenchmarkID id) {
        const auto& benchmarkType = nostalgia::benchmark::atlas.at(id);
        if (benchmarkType.dispatcher) {
            benchmarkType.dispatcher();
        }
    }
}