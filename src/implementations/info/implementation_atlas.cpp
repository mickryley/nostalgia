#include "implementation_atlas.h"

namespace nostalgia::implementation {
	std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas{
		{ 
			// === Global Access Implementations ===
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Container Forward Deallocation",
				"Object Overload (Global Referencing to Allocator) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Container Reverse Deallocation",
				"Object Overload (Global Referencing to Allocator) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Container Rewind Deallocation",
				"Object Overload (Global Referencing) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Vector Rewind Deallocation",
				"Object Overload (Global Referencing) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Singleton Access Implementation ===
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Container Forward Deallocation",
				"Object Overload (Singleton Referencing) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Container Reverse Deallocation",
				"Object Overload (Singleton Referencing) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Container Rewind Deallocation",
				"Object Overload (Singleton Referencing) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Cached Singleton Pointer Container Rewind Deallocation",
				"Object Overload (Cached Singleton Referencing) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Vector Rewind Deallocation",
				"Object Overload (Singleton Referencing) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Cached Singleton Pointer Vector Rewind Deallocation",
				"Object Overload (Cached Singleton Referencing) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},


			// === Malloc Implementations ===
			{ nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Default Allocator Malloc Pointer Container Forward Deallocation",
				"Default Allocator Malloc [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEFAULT_MALLOC | AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Default Allocator Malloc Pointer Container Reverse Deallocation",
				"Default Allocator Malloc [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEFAULT_MALLOC | AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::NoAllocator_Malloc_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Default Allocator Malloc Pointer Vector Reverse Deallocation",
				"Default Allocator Malloc [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEFAULT_MALLOC | AllocatorFlags::DEALLOC_REVERSE
				}
			}
		}
	};
}