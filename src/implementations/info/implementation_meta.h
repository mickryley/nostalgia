#pragma once

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include <string>
#include <functional>

#include "utils/log.h"

namespace nostalgia {

	enum class ImplementationID {
		NONE,

		Templated_GlobalAccess_PointerBlock_RewindDeallocation,
		Templated_GlobalAccess_PointerContainer_RewindDeallocation,
		Templated_GlobalAccess_PointerVector_RewindDeallocation,


		ObjectOverride_GlobalAccess_PointerContainer_ForwardDeallocation,
		ObjectOverride_GlobalAccess_PointerContainer_ReverseDeallocation,

		ObjectOverride_GlobalAccess_PointerContainer_RewindDeallocation,
		ObjectOverride_GlobalAccess_PointerVector_RewindDeallocation,


		ObjectOverride_SingletonAccess_PointerContainer_ForwardDeallocation,
		ObjectOverride_SingletonAccess_PointerContainer_ReverseDeallocation,

		ObjectOverride_SingletonAccess_PointerContainer_RewindDeallocation,
		ObjectOverride_CachedSingletonAccess_PointerContainer_RewindDeallocation,
		ObjectOverride_SingletonAccess_PointerVector_RewindDeallocation,
		ObjectOverride_CachedSingletonAccess_PointerVector_RewindDeallocation,

		NoAllocator_Malloc_PointerContainer_ForwardDeallocation,
		NoAllocator_Malloc_PointerContainer_ReverseDeallocation,
		NoAllocator_Malloc_PointerVector_ReverseDeallocation
	};

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