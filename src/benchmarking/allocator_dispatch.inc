#include "allocators/allocator_meta"

#include "log.h"

switch (allocator.id) {
    #define DISPATCH(id, ns) case id: ns::RUN_BENCHMARK; break;

    DISPATCH(AllocatorID::Linear, linear)
    DISPATCH(AllocatorID::Stack, stack)
    DISPATCH(AllocatorID::Pool, pool)

    #undef DISPATCH
    default: log::print(logflags::ERROR, "Unknown allocator.");
}