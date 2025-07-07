#pragma once
#include "benchmarking/benchmark_params.h"
#include <vector>

namespace nostalgia::gui{
    int init_gui();

	void draw_gui();

	void load_benchmarking_params(const std::vector<BenchmarkParamSpec>& specs, nostalgia::BenchmarkParams& outputParams);

	void draw_leftPanel();
	void draw_topBar();
	void draw_mainPage();
}