#pragma once

#include "implementation_meta.h"
#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include <unordered_map>

namespace nostalgia::implementation {
	extern std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas;

	inline nostalgia::AllocatorFlags get_compatible_flags(nostalgia::ImplementationID id) {
		if (atlas.contains(id)) return atlas.at(id).compatible_flags;
		return nostalgia::AllocatorFlags::NONE;
	}

	inline bool is_compatible_with_allocator(nostalgia::ImplementationID i_id, nostalgia::AllocatorID a_id) {
		if (allocator::atlas.contains(a_id)) {
			return allocator::atlas.at(a_id).is_compatible_with_implementation(get_compatible_flags(i_id));
		}
		return false;
	} 

	inline std::string get_label(nostalgia::ImplementationID id) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.label;
		}
		return "Unknown Implementation";
	}
}