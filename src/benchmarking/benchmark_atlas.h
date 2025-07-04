#pragma once

#include "benchmark_meta.h"

#include <unordered_map>

namespace nostalgia::benchmark {
	extern std::unordered_map<nostalgia::BenchmarkID, nostalgia::BenchmarkType> atlas;

	inline nostalgia::AllocatorFlags getCompatibleFlags(nostalgia::BenchmarkID id) {
		auto it = atlas.find(id);
		if (it != atlas.end()) {
			return it->second.compatibleFlags;
		}
		return nostalgia::AllocatorFlags::NONE;
	}
}