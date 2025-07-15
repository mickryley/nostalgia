#pragma once
#include "benchmarking/info/benchmark_params.h"
#include "utils/return_codes.h"

#include <vector>

namespace nostalgia::gui{
    ReturnCode init_gui();
	void load_benchmarking_params(const std::vector<BenchmarkParamSpec>& specs);

	void draw_gui();
	void draw_browser_panel();
	void draw_header_bar();
	void draw_main_page();
	void draw_setup_new_benchmark();
	void draw_hover_details();
}