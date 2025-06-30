#include "gui.h"

#include "render.h"

#include "allocators/allocator_meta.h"
#include "allocators/linear_bump/allocator_linear.h"

#include "benchmarking/benchmarking_manager.h"

#include <iostream>

namespace nostalgia{
    int init_gui () {
        std::cout << "init_gui" << "\n"; 

        benchmarking::init_benchmarking_manager();

        init_render();

        return 0;
    };
}