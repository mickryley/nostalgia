#include "ibmbursts.h" 
#include "utils/shared_benchmark_includes.inl"

struct ImplementationDetails {
	const char* allocator;
	const char* name;
	const char* desc;
};

namespace nostalgia::benchmarking::IBMBursts {

	namespace default_malloc{
#include "benchmarking/ibmbursts/ibmbursts_forward_dec.inl"
#include "allocators/defines/malloc_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts_object_switch.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace linear {
#include "benchmarking/ibmbursts/ibmbursts_forward_dec.inl"
#include "allocators/defines/linear_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts_object_switch.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace stack {
#include "benchmarking/ibmbursts/ibmbursts_forward_dec.inl"
#include "allocators/defines/stack_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts_object_switch.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace pool {
#include "benchmarking/ibmbursts/ibmbursts_forward_dec.inl"
#include "allocators/defines/pool_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts_object_switch.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace freelist {
#include "benchmarking/ibmbursts/ibmbursts_forward_dec.inl"
#include "allocators/defines/freelist_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts_object_switch.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
}