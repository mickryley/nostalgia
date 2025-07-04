#include "implementation_atlas.h"

namespace nostalgia::implementation {
	std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas{
		{ 
			{ nostalgia::ImplementationID::OO_St_cP_FD, nostalgia::ImplementationType{
				"Object Overload Static Cont of Pointers Forward Deallocation",
				"Object Overload (Static Referencing) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::OO_St_cP_RD, nostalgia::ImplementationType{
				"Object Overload Static Cont of Pointers Reverse Deallocation",
				"Object Overload (Static Referencing) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::OO_St_cP_RwD, nostalgia::ImplementationType{
				"Object Overload Static Cont of Pointers Rewind Deallocation",
				"Object Overload (Static Referencing) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			}
		}
	};
}