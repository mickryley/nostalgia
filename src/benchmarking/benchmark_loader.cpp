#include "benchmarking/benchmark_loader.h"
#include "benchmarking/benchmark_atlas.h"

#include "allocators/allocator_atlas.h"

#include "implementations/implementation_atlas.h"

#include "ui/gui.h"

#include "log.h"
#include <unordered_set>

namespace nostalgia::benchmarking::loader {

    namespace {
        nostalgia::BenchmarkParams m_paramOutput;
        nostalgia::BenchmarkID m_currentBenchmarkID = nostalgia::BenchmarkID::NONE;
		std::unordered_set<nostalgia::AllocatorID> m_selectedAllocators;
        std::unordered_set<nostalgia::ImplementationID> m_selectedImplementations;
    }

    nostalgia::BenchmarkID getBenchmarkID() {
        return m_currentBenchmarkID;
    }


    nostalgia::AllocatorFlags getAllocatorFlags() {
		return nostalgia::benchmark::atlas.at(m_currentBenchmarkID).compatibleFlags;
    }

    void addAllocatorToBenchmark(const nostalgia::AllocatorID id, bool add) {
        if (add && !m_selectedAllocators.contains(id)) m_selectedAllocators.insert(id);
        else if (!add && m_selectedAllocators.contains(id)) m_selectedAllocators.erase(id);
    }

    bool isAllocatorInBenchmark(const nostalgia::AllocatorID id) {
        return m_selectedAllocators.contains(id);
    }

    void addImplementationToBenchmark(const nostalgia::ImplementationID id, bool add) {
        if (add && !m_selectedImplementations.contains(id)) m_selectedImplementations.insert(id);
        else if (!add && m_selectedImplementations.contains(id)) m_selectedImplementations.erase(id);
    }

    bool isImplementationInBenchmark(const nostalgia::ImplementationID id) {
        return m_selectedImplementations.contains(id);
    }

    nostalgia::BenchmarkParams& getParameters() {
        return m_paramOutput;
    }

    void loadBenchmark(const nostalgia::BenchmarkID id) {
        log::print("Loading benchmark...");

        m_currentBenchmarkID = id;
		m_selectedAllocators.clear();
        m_selectedImplementations.clear();

        // Prefill with compatible allocators
        for (const auto& [aID, aType] : nostalgia::allocator::atlas) {
            if (aType.isCompatibleWith(getAllocatorFlags()))
				m_selectedAllocators.insert(aID);
        }

        // Prefill with ALL implementations
        // Room to add filter for benchmark + selected allocators in future.
        for (const auto& [iID, iType] : nostalgia::implementation::atlas) {
            //if (iType.isCompatibleWith(getAllocatorFlags()))
                m_selectedImplementations.insert(iID);
        }

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