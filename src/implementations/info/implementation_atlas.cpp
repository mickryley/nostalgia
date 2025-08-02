#include "implementation_atlas.h"

namespace nostalgia::implementation {
	std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas{
		{ 
			// === Templated Implementations - Global Access ===
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerBlock_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Block Forward Deallocation",
				"Templated (Global Referencing to Allocator) [Contiguous Block of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerBlock_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Block Reverse Deallocation",
				"Templated (Global Referencing to Allocator) [Contiguous Block of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerBlock_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Block Rewind Deallocation",
				"Templated (Global Referencing to Allocator) [Contiguous Block of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND | AllocatorFlags::BULK_ALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Container Forward Deallocation",
				"Templated (Global Referencing to Allocator) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Container Reverse Deallocation",
				"Templated (Global Referencing to Allocator) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Container Rewind Deallocation",
				"Templated (Global Referencing to Allocator) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Vector Forward Deallocation",
				"Templated (Global Referencing to Allocator) [std Vector of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Vector Reverse Deallocation",
				"Templated (Global Referencing to Allocator) [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_GlobalAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Global Access Pointer Vector Rewind Deallocation",
				"Templated (Global Referencing to Allocator) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Templated Implementations - Singleton Access ===
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerBlock_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Block Forward Deallocation",
				"Templated (Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerBlock_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Block Reverse Deallocation",
				"Templated (Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerBlock_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Block Rewind Deallocation",
				"Templated (Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND | AllocatorFlags::BULK_ALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Container Forward Deallocation",
				"Templated (Singleton Referencing to Allocator) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Container Reverse Deallocation",
				"Templated (Singleton Referencing to Allocator) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Container Rewind Deallocation",
				"Templated (Singleton Referencing to Allocator) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerVector_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Vector Forward Deallocation",
				"Templated (Singleton Referencing to Allocator) [std Vector of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Vector Reverse Deallocation",
				"Templated (Singleton Referencing to Allocator) [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_SingletonAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Singleton Access Pointer Vector Rewind Deallocation",
				"Templated (Singleton Referencing to Allocator) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Templated Implementations - Cached Singleton Access ===
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerBlock_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Block Forward Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerBlock_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Block Reverse Deallocation",
				"Templated (Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE | AllocatorFlags::BULK_ALLOCATE | AllocatorFlags::SUB_DEALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerBlock_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Block Rewind Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [Contiguous Block of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND | AllocatorFlags::BULK_ALLOCATE
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerContainer_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Container Forward Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [Containers of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerContainer_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Container Reverse Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [Containers of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Container Rewind Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [Containers of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerVector_ForwardDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Vector Forward Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [std Vector of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Vector Reverse Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::Templated_CachedSingletonAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Templated Cached Singleton Access Pointer Vector Rewind Deallocation",
				"Templated (Cached Singleton Referencing to Allocator) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Object Override - Global Access Implementations ===
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
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ForwardDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Vector Forward Deallocation",
				"Object Overload (Global Referencing) [std Vector of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Vector Reverse Deallocation",
				"Object Overload (Global Referencing) [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Global Access Pointer Vector Rewind Deallocation",
				"Object Overload (Global Referencing) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Object Override - Singleton Access Implementations ===
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

			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ForwardDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Vector Forward Deallocation",
				"Object Overload (Singleton Referencing) [std Vector of Pointers] [Forward Deallocation]",
				AllocatorFlags::DEALLOC_FORWARD
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ReverseDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Vector Reverse Deallocation",
				"Object Overload (Singleton Referencing) [std Vector of Pointers] [Reverse Deallocation]",
				AllocatorFlags::DEALLOC_REVERSE
				}
			},
			{ nostalgia::ImplementationID::ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Singleton Pointer Vector Rewind Deallocation",
				"Object Overload (Singleton Referencing) [std Vector of Pointers] [Rewind Deallocation]",
				AllocatorFlags::DEALLOC_REWIND
				}
			},

			// === Object Override - Cached Singleton Access Implementations ===
			// Cached  Singleton Access only benefits rewinds in Object Override implementations
			// ~~~ Rewind Deallocations ~~~
			{ nostalgia::ImplementationID::ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation, nostalgia::ImplementationType{
				"Object Overload Cached Singleton Pointer Container Rewind Deallocation",
				"Object Overload (Cached Singleton Referencing) [Containers of Pointers] [Rewind Deallocation]",
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