#include "benchmark_atlas.h"
#include "benchmarking/dispatcher/benchmark_loader.h"
#include "benchmarking/dispatcher/dispatcher.h"

namespace nostalgia::benchmark {
std::unordered_map<nostalgia::BenchmarkID, nostalgia::benchmark_type> atlas{
        {
            nostalgia::BenchmarkID::IBM_Bursts,
            nostalgia::benchmark_type{
                .label = "IBM Bursts",
                .label_long = "IBM Bursts of repeated Alloc and Dealloc",
                .description = "This popular microbenchmark method runs repeated passes/bursts of allocation up to a defined number and then deallocation of the entire allocated amount.\n\nThis is typically expecting a like-for-like test of reverse (LIFO) deallocation.\n\nHowever the benchmark is technically compatible with numerous deallocation methods (despite failing like-for-like featuresets).\n\nTherefore for completeness, this benchmark's allocation method is always forward incremented, while the deallocation methods are permitted to include reverse deallocation, rewind, and forward. This enables the comparison of a wider variety of allocators, such as rewind-only varieties.",
                .required_flags = AllocatorFlags::NONE,
                .disabled = false,
                .dispatcher = []() {
                    nostalgia::benchmarking::IBMBursts::dispatch();
                },
                .loader = []() {
                    nostalgia::benchmarking::loader::load_benchmark(
                        nostalgia::BenchmarkID::IBM_Bursts);
                },
                .paramSpecs = {
					{"passes", "int", "How many passes of the benchmark to run.", true, "5000"},
					{"iterations", "int", "How many allocations + dealloactions to run.", true, "1000"},
					{"object", "object", "Which Object to use for Allocation", false, "1"},
				}
            }
        }
    };

}