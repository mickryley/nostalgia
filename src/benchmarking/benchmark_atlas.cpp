#include "benchmark_atlas.h"
#include "benchmark_loader.h"
#include "allocator_dispatch.h"

namespace nostalgia::benchmark {
std::unordered_map<nostalgia::BenchmarkID, nostalgia::BenchmarkType> atlas{
        {
            nostalgia::BenchmarkID::IBM_Bursts,
            nostalgia::BenchmarkType{
                .label = "IBM Bursts",
                .description = "IBM Bursts Of Alloc and Dealloc",
                .compatibleFlags = AllocatorFlags::NONE,
                .disabled = false,
                .dispatcher = []() {
                    nostalgia::benchmarking::IBMBursts::dispatch();
                },
                .loader = []() {
                    nostalgia::benchmarking::loader::loadBenchmark(
                        nostalgia::BenchmarkID::IBM_Bursts);
                },
                .paramSpecs = {
					{"passes", "int", "How many passes of the benchmark to run.", true, "5000"},
					{"iterations", "int", "How many allocations + dealloactions to run.", true, "1000"},
					{"object", "int", "Which Object to Allocator", false, "0"},
				}
            }
        }
    };

}