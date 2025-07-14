#pragma once

#include "implementation_meta.h"
#include "allocators/info/allocator_meta.h"

#include <unordered_map>

namespace nostalgia::implementation {
	extern std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas;

	inline bool is_compatible_with_allocator(nostalgia::ImplementationID id, nostalgia::AllocatorType allocator) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.is_compatible_with(allocator.required_flags);
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