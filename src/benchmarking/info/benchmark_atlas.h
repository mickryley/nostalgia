#pragma once
#include "benchmark_meta.h"
#include <unordered_map>

namespace nostalgia::benchmark {
	extern std::unordered_map<nostalgia::BenchmarkID, nostalgia::benchmark_type> atlas;
	inline nostalgia::AllocatorFlags get_compatible_flags(nostalgia::BenchmarkID id) {
		if (atlas.contains(id)) return atlas.at(id).compatible_flags;
		return nostalgia::AllocatorFlags::NONE;
	}
}