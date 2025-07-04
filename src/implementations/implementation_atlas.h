#pragma once

#include "implementation_meta.h"
#include "allocators/allocator_meta.h"

#include <unordered_map>

namespace nostalgia::implementation {
	extern std::unordered_map<nostalgia::ImplementationID, nostalgia::ImplementationType> atlas;

	inline bool isCompatibleWithAllocator(nostalgia::ImplementationID id, nostalgia::AllocatorType allocator) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.isCompatibleWith(allocator.requiredFlags);
		}
		return false;
	}

	inline std::string getLabel(nostalgia::ImplementationID id) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.label;
		}
		return "Unknown Implementation";
	}
}