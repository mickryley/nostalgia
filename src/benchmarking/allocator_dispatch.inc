switch (allocator.id) {
#define DISPATCH(id, ns) case id: RUN_BENCHMARK(ns); break;

	DISPATCH(AllocatorID::Linear, linear) // Would result in benchmarking::IBMBursts::linear::run_ibmbursts_benchmark();
        DISPATCH(AllocatorID::Stack, stack)
        DISPATCH(AllocatorID::Pool, pool)
        DISPATCH(AllocatorID::Freelist, freelist)

#undef DISPATCH
        default: log::print(logFlags::ERROR, "Unknown allocator.");
        }