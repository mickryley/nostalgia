#include "implementation_atlas.h"

namespace nostalgia::implementation {
	std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas{
		{ 
			{ nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Object Overload Static Access Pointer Container Forward Deallocation",
				"Object Overload (Static Referencing to Allocator) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Object Overload Static Access Pointer Container Reverse Deallocation",
				"Object Overload (Static Referencing to Allocator) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Static Access Pointer Container Rewind Deallocation",
				"Object Overload (Static Referencing) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Default Allocator Malloc Pointer Container Forward Deallocation",
				"Default Allocator Malloc [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEFAULT_MALLOC | AllocatorFlags::DEALLOC_FORWARD
				}
			}
		}
	};
}