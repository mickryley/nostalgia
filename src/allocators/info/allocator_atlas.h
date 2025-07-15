#pragma once
#include "allocator_meta.h"
#include <unordered_map>

namespace nostalgia::allocator {
	extern std::unordered_map<nostalgia::AllocatorID, nostalgia::AllocatorType> atlas;

	inline std::string get_label(nostalgia::AllocatorID id) {
		if (atlas.contains(id)) return atlas.at(id).label;
		return "Unknown Allocator";
	};
}