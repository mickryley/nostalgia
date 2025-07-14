#pragma once
#include "utils/return_codes.h"

namespace nostalgia::headless {
    ReturnCode init_headless(const int argc, char** argv);
}