#include "allocator_atlas.h"

namespace nostalgia::allocator {
	std::unordered_map<nostalgia::AllocatorID, nostalgia::AllocatorType> atlas{
		{ { nostalgia::AllocatorID::Linear, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Linear,
			"Linear",
			"Basic Minimal Linear Allocator (Rewind Only)",
			AllocatorFlags::DEALLOC_REWIND
			}
		},
			{ nostalgia::AllocatorID::Stack, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Stack,
			"Stack",
			"Basic Minimal LIFO Stack Allocator",
			AllocatorFlags::DEALLOC_REVERSE
			}
		},
		 {	nostalgia::AllocatorID::Pool, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Pool,
			"Pool",
			"Fixed Size Pre-Formatted Pool Allocator",
			AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::DEALLOC_FORWARD
			}
		},
		{ nostalgia::AllocatorID::Freelist, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Freelist,
			"Freelist",
			"Variable Size Freelist Allocator (Coalesces on Deallocate)",
			AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::DEALLOC_FORWARD |
			AllocatorFlags::DEALLOC_RANDOM
			}
		} }
	};
}