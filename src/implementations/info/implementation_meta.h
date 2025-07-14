#pragma once

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include <string>
#include <functional>

#include "utils/log.h"

namespace nostalgia {

	enum class ImplementationID {
		NONE,
		ObjectOverride_StaticAccess_PointerContainer_ForwardDeallocation,
		ObjectOverride_StaticAccess_PointerContainer_ReverseDeallocation,
		ObjectOverride_StaticAccess_PointerContainer_RewindDeallocation,
		NoAllocator_Malloc_PointerContainer_ForwardDeallocation
	};

	struct ImplementationType{
		const char* label;
		const char* desc;

		const AllocatorFlags compatible_flags;

		bool compatible(AllocatorFlags flags) const {
			return (compatible_flags & flags) == flags;
		};

		bool is_compatible_with(AllocatorFlags flags) const{
			// return false;
			return (compatible_flags & flags) == compatible_flags;
		};
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