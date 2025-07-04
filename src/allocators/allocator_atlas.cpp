#include "allocator_atlas.h"

namespace nostalgia::allocator {
	std::unordered_map<nostalgia::AllocatorID, nostalgia::AllocatorType> atlas{
		{ { nostalgia::AllocatorID::Linear, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Linear,
			"Linear",
			"Linear",
			AllocatorFlags::DEALLOC_REWIND
			}
		},
			{ nostalgia::AllocatorID::Stack, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Stack,
			"Stack",
			"Stack",
			AllocatorFlags::DEALLOC_REVERSE
			}
		},
		 {	nostalgia::AllocatorID::Pool, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Pool,
			"Pool",
			"Pool",
			AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::DEALLOC_FORWARD
			}
		},
		{ nostalgia::AllocatorID::Freelist, nostalgia::AllocatorType{
			nostalgia::AllocatorID::Freelist,
			"Freelist",
			"Freelist",
			AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::DEALLOC_FORWARD |
			AllocatorFlags::DEALLOC_RANDOM
			}
		} }
	};
}