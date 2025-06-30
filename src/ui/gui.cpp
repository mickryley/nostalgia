#include "gui.h"

#include "render.h"

#include "allocators/allocator_meta.h"
#include "allocators/linear_bump/allocator_linear.h"

#include <iostream>

namespace nostalgia{
    int init_gui () {
        std::cout << "init_gui" << "\n"; 
        init_render();
        return 0;
    };
}