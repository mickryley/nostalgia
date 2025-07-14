// This .inc conforms and forwards a benchmark to all ::allocator:: namespaces
// Example Result ~~~ benchmarking::IBMBursts::linear::run_ibmbursts_benchmark(); ~~~

switch (allocator.id) {
#define DISPATCH(id, ns) case id: RUN_BENCHMARK(ns); break;
        
	DISPATCH(AllocatorID::Linear, linear) 
        DISPATCH(AllocatorID::Stack, stack)
        DISPATCH(AllocatorID::Pool, pool)
        DISPATCH(AllocatorID::Freelist, freelist)

#undef DISPATCH
        default: log::print(LogFlags::Error, "Unknown allocator.");
        }