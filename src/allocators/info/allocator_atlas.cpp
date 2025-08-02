#include "allocator_atlas.h"

namespace nostalgia::allocator {

    std::unordered_map<nostalgia::AllocatorID, nostalgia::AllocatorType> atlas{
        { nostalgia::AllocatorID::NONE, nostalgia::AllocatorType{
            .id = nostalgia::AllocatorID::NONE,
            .label = "No Allocator (Default)",
            .description = "Malloc Allocator (Default)",
            .compatible_flags = AllocatorFlags::DEALLOC_REVERSE |
                                AllocatorFlags::DEALLOC_FORWARD |
                                AllocatorFlags::DEALLOC_RANDOM,
            .required_flags = AllocatorFlags::DEFAULT_MALLOC,
            .data_color_hue_shift = 0
        }},
        { nostalgia::AllocatorID::Linear, nostalgia::AllocatorType{
            .id = nostalgia::AllocatorID::Linear,
            .label = "Linear",
            .description = "Basic Minimal Linear Allocator (Rewind Only)",
            .compatible_flags = AllocatorFlags::DEALLOC_REWIND,
            .required_flags =   AllocatorFlags::NONE |
                                AllocatorFlags::BULK_ALLOCATE,
            .data_color_hue_shift = 40
        }},
        { nostalgia::AllocatorID::Stack, nostalgia::AllocatorType{
            .id = nostalgia::AllocatorID::Stack,
            .label = "Stack",
            .description = "Basic Minimal LIFO Stack Allocator",
            .compatible_flags = AllocatorFlags::DEALLOC_REVERSE |
                                AllocatorFlags::SUB_DEALLOCATE,
            .required_flags = AllocatorFlags::NONE,
            .data_color_hue_shift = 80
        }},
        { nostalgia::AllocatorID::Pool, nostalgia::AllocatorType{
            .id = nostalgia::AllocatorID::Pool,
            .label = "Pool",
            .description = "Fixed Size Pre-Formatted Pool Allocator",
            .compatible_flags = AllocatorFlags::DEALLOC_REVERSE |
                                AllocatorFlags::DEALLOC_FORWARD |
                                AllocatorFlags::SUB_DEALLOCATE,
            .required_flags = AllocatorFlags::NONE,
            .data_color_hue_shift = 120
        }},
        { nostalgia::AllocatorID::Freelist, nostalgia::AllocatorType{
            .id = nostalgia::AllocatorID::Freelist,
            .label = "Freelist",
            .description = "Variable Size Freelist Allocator (Coalesces on Deallocate)",
            .compatible_flags = AllocatorFlags::DEALLOC_REVERSE |
                                AllocatorFlags::DEALLOC_FORWARD |
                                AllocatorFlags::DEALLOC_RANDOM,
            .required_flags = AllocatorFlags::NONE,
            .data_color_hue_shift = 160
        }}
    };
}
