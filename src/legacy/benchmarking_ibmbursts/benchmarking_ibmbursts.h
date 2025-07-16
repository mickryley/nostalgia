#pragma once

#include "benchmarking/benchmarking_manager.h"

namespace nostalgia::benchmarking::IBMBursts {

	void benchmark_IBMBursts_linearAllocators();
	void benchmark_IBMBursts_stackAllocators();
	void benchmark_IBMBurstg_pool_allocators();
	void benchmark_IBMBursts_freelistAllocators();

}