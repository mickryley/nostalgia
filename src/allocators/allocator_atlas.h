#pragma once

#include "allocator_meta.h"

#include <unordered_map>

namespace nostalgia::allocator {
	extern std::unordered_map<nostalgia::AllocatorID, nostalgia::AllocatorType> atlas;

	inline std::string getLabel(nostalgia::AllocatorID id) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.label;
		}
		return "Unknown Allocator";
	};
}