#pragma once

#include "allocators/allocator_meta.h"

#include "allocators/allocator_atlas.h"
//#include "implementations/implementation_atlas.h"

#include <string>
#include <functional>

#include "log.h"

namespace nostalgia {

	enum class ImplementationID {
		NONE,
		OO_St_cP_FD,
		OO_St_cP_RD,
		OO_St_cP_RwD
	};

	struct ImplementationType{
		const char* label;
		const char* desc;

		const AllocatorFlags compatibleFlags;

		bool compatible(AllocatorFlags flags) const {
			return (compatibleFlags & flags) == flags;
		};

		bool isCompatibleWith(AllocatorFlags flags) const{
			// return false;
			return (compatibleFlags & flags) == compatibleFlags;
		};
	};


	// Details store a type plus passed parameters
	struct ImplementationDetails {
		const ImplementationID id; // To return the implementation type
		const AllocatorID allocator;
		std::string parameters;

		const std::function<void()> run;

		std::string label() const {
			return std::format("{} - {}",
				nostalgia::allocator::getLabel(allocator),
				parameters);
		};
		/*bool compatible(AllocatorFlags flags) const {
			return (compatibleFlags & flags) == flags;
		}*/
	};
}