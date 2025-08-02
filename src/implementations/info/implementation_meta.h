#pragma once

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include <string>
#include <functional>

#include "utils/log.h"
#include "utils/json_wrapped.h"

namespace nostalgia {

	enum class ImplementationID {
		NONE,
		// === Templated Global Access ===
		Templated_GlobalAccess_PointerBlock_ForwardDeallocation,
		Templated_GlobalAccess_PointerBlock_ReverseDeallocation,
		Templated_GlobalAccess_PointerBlock_RewindDeallocation,

		Templated_GlobalAccess_PointerContainer_ForwardDeallocation,
		Templated_GlobalAccess_PointerContainer_ReverseDeallocation,
		Templated_GlobalAccess_PointerContainer_RewindDeallocation,

		Templated_GlobalAccess_PointerVector_ForwardDeallocation,
		Templated_GlobalAccess_PointerVector_ReverseDeallocation,
		Templated_GlobalAccess_PointerVector_RewindDeallocation,

		// === Templated Singleton Access ===
		Templated_SingletonAccess_PointerBlock_ForwardDeallocation,
		Templated_SingletonAccess_PointerBlock_ReverseDeallocation,
		Templated_SingletonAccess_PointerBlock_RewindDeallocation,

		Templated_SingletonAccess_PointerContainer_ForwardDeallocation,
		Templated_SingletonAccess_PointerContainer_ReverseDeallocation,
		Templated_SingletonAccess_PointerContainer_RewindDeallocation,

		Templated_SingletonAccess_PointerVector_ForwardDeallocation,
		Templated_SingletonAccess_PointerVector_ReverseDeallocation,
		Templated_SingletonAccess_PointerVector_RewindDeallocation,
		// === Templated Cached Singleton Access ===
		Templated_CachedSingletonAccess_PointerBlock_ForwardDeallocation,
		Templated_CachedSingletonAccess_PointerBlock_ReverseDeallocation,
		Templated_CachedSingletonAccess_PointerBlock_RewindDeallocation,

		Templated_CachedSingletonAccess_PointerContainer_ForwardDeallocation,
		Templated_CachedSingletonAccess_PointerContainer_ReverseDeallocation,
		Templated_CachedSingletonAccess_PointerContainer_RewindDeallocation,

		Templated_CachedSingletonAccess_PointerVector_ForwardDeallocation,
		Templated_CachedSingletonAccess_PointerVector_ReverseDeallocation,
		Templated_CachedSingletonAccess_PointerVector_RewindDeallocation,

		ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation,
		ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation,

		ObjectOverride_GlobalAccess_PointerVector_ForwardDeallocation,
		ObjectOverride_GlobalAccess_PointerVector_ReverseDeallocation,

		ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation,
		ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation,

		ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation,
		ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation,
		ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation,

		ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation,

		ObjectOverride_SingletonAccess_PointerVector_ForwardDeallocation, // In Progress
		ObjectOverride_SingletonAccess_PointerVector_ReverseDeallocation, // In Progress
		ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation,

		ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation,

		NoAllocator_Malloc_PointerContainer_ForwardDeallocation,
		NoAllocator_Malloc_PointerContainer_ReverseDeallocation,
		NoAllocator_Malloc_PointerVector_ReverseDeallocation
	};

	NLOHMANN_JSON_SERIALIZE_ENUM(ImplementationID, {
		{ ImplementationID::NONE, "NONE" },

		// === Templated Global Access ===
		// ~~~ Pointer Block ~~~
		{ ImplementationID::Templated_GlobalAccess_PointerBlock_ForwardDeallocation, "Templated_GlobalAccess_PointerBlock_ForwardDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerBlock_ReverseDeallocation, "Templated_GlobalAccess_PointerBlock_ReverseDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerBlock_RewindDeallocation, "Templated_GlobalAccess_PointerBlock_RewindDeallocation" },

		// ~~~ Pointer Container ~~~
		{ ImplementationID::Templated_GlobalAccess_PointerContainer_ForwardDeallocation, "Templated_GlobalAccess_PointerContainer_ForwardDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerContainer_ReverseDeallocation, "Templated_GlobalAccess_PointerContainer_ReverseDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerContainer_RewindDeallocation, "Templated_GlobalAccess_PointerContainer_RewindDeallocation" },

		// ~~~ Pointer Vector ~~~
		{ ImplementationID::Templated_GlobalAccess_PointerVector_ForwardDeallocation, "Templated_GlobalAccess_PointerVector_ForwardDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerVector_ReverseDeallocation, "Templated_GlobalAccess_PointerVector_ReverseDeallocation" },
		{ ImplementationID::Templated_GlobalAccess_PointerVector_RewindDeallocation, "Templated_GlobalAccess_PointerVector_RewindDeallocation" },
		
		// === Templated Singleton Access ===
		// ~~~ Pointer Block ~~~
		{ ImplementationID::Templated_SingletonAccess_PointerBlock_ForwardDeallocation, "Templated_SingletonAccess_PointerBlock_ForwardDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerBlock_ReverseDeallocation, "Templated_SingletonAccess_PointerBlock_ReverseDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerBlock_RewindDeallocation, "Templated_SingletonAccess_PointerBlock_RewindDeallocation" },

		// ~~~ Pointer Container ~~~
		{ ImplementationID::Templated_SingletonAccess_PointerContainer_ForwardDeallocation, "Templated_SingletonAccess_PointerContainer_ForwardDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerContainer_ReverseDeallocation, "Templated_SingletonAccess_PointerContainer_ReverseDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerContainer_RewindDeallocation, "Templated_SingletonAccess_PointerContainer_RewindDeallocation" },

		// ~~~ Pointer Vector ~~~
		{ ImplementationID::Templated_SingletonAccess_PointerVector_ForwardDeallocation, "Templated_SingletonAccess_PointerVector_ForwardDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerVector_ReverseDeallocation, "Templated_SingletonAccess_PointerVector_ReverseDeallocation" },
		{ ImplementationID::Templated_SingletonAccess_PointerVector_RewindDeallocation, "Templated_SingletonAccess_PointerVector_RewindDeallocation" },

		{ ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation, "ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation" },
		{ ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation, "ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation" },

		{ ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ForwardDeallocation, "ObjectOverride_GlobalAccess_PointerVector_ForwardDeallocation" },
		{ ImplementationID::ObjectOverride_GlobalAccess_PointerVector_ReverseDeallocation, "ObjectOverride_GlobalAccess_PointerVector_ReverseDeallocation" },

		{ ImplementationID::ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation, "ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation" },
		{ ImplementationID::ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation, "ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation" },

		{ ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation, "ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation" },
		{ ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation, "ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation" },
		{ ImplementationID::ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation, "ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation" },

		{ ImplementationID::ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation, "ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation" },
		
		{ ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ForwardDeallocation, "ObjectOverride_SingletonAccess_PointerVector_ForwardDeallocation" },
		{ ImplementationID::ObjectOverride_SingletonAccess_PointerVector_ReverseDeallocation, "ObjectOverride_SingletonAccess_PointerVector_ReverseDeallocation" },
		{ ImplementationID::ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation, "ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation" },
		
		{ ImplementationID::ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation, "ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation" },

		{ ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation, "NoAllocator_Malloc_PointerContainer_ForwardDeallocation" },
		{ ImplementationID::NoAllocator_Malloc_PointerContainer_ReverseDeallocation, "NoAllocator_Malloc_PointerContainer_ReverseDeallocation" },
		{ ImplementationID::NoAllocator_Malloc_PointerVector_ReverseDeallocation, "NoAllocator_Malloc_PointerVector_ReverseDeallocation" }
	});


	struct ImplementationType{
		const char* label;
		const char* desc;

		const AllocatorFlags compatible_flags;
	};

	struct ImplementationDetails {
		const ImplementationID id; 
		const AllocatorID allocator;
		std::string parameters;

		const std::function<void()> run;

		std::string label() const {
			return std::format("{} - {}",
				nostalgia::allocator::get_label(allocator),
				parameters);
		};
	};
}