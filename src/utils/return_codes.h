#pragma once

namespace nostalgia {
	enum class ReturnCode {
		Success = 0,
        InitHeadlessUnimplementedError = 1,
        InitGuiError = 2,
		InitGuiSDL3Error = 3,
		InitGuiVulkanError = 4,
	};
}