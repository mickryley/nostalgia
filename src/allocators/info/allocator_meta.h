#pragma once

#include "utils/json_wrapped.h"

#include "utils/log.h"

namespace nostalgia {
    enum class AllocatorID {
        NONE = 0,
        Linear,
        Stack,
        Pool,
        Freelist
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(AllocatorID, {
        { AllocatorID::NONE, "NONE" },
        { AllocatorID::Linear, "Linear" },
        { AllocatorID::Stack, "Stack" },
        { AllocatorID::Pool, "Pool" },
        { AllocatorID::Freelist, "Freelist" }
    
    });

    enum class AllocatorFlags : uint32_t {
        NONE = 0,
        FIXED_SIZE              = 1 << 0,
        VARIABLE_SIZE           = 1 << 1,
        THREAD_SAFE             = 1 << 2,
		DEALLOC_REWIND          = 1 << 3,
		DEALLOC_REVERSE         = 1 << 4,
		DEALLOC_FORWARD         = 1 << 5,
		DEALLOC_RANDOM          = 1 << 6,
        DEFAULT_MALLOC          = 1 << 7,
        BULK_ALLOCATE           = 1 << 8, 
        SUB_DEALLOCATE          = 1 << 9, 

    };
    ENABLE_BITMASK_OPERATORS(AllocatorFlags);

    struct AllocatorType {
        const AllocatorID id;
        const std::string label;
        const std::string description;
        const AllocatorFlags compatible_flags;
        const AllocatorFlags required_flags;
		const int data_color_hue_shift; 

        bool has(AllocatorFlags flag) const {
            return hasFlag(compatible_flags, flag);
        }

        bool is_compatible_with_benchmark(AllocatorFlags flags) const {
			return hasOnlyCompatibleFlags(flags, compatible_flags);
        }

        bool is_compatible_with_implementation(AllocatorFlags flags) const {
            bool passes_required = hasAllRequiredFlags(flags, required_flags);
            bool passes_compatible = hasOnlyCompatibleFlags(flags, compatible_flags | required_flags);
			return (passes_required && passes_compatible);
		}
    };
}