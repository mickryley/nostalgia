#include "ui/headless/headless.h"
#include "ui/gui/gui.h"
#include "utils/return_codes.h"
#include <string_view>

namespace {
    bool is_headless(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i)
            if (std::string_view(argv[i]) == "-headless")
                return true;
        return false;
    }
}

int main(int argc, char** argv) {
    if (is_headless(argc, argv) )
        return static_cast<int>(nostalgia::headless::init_headless(argc, argv));
    return  static_cast<int>(nostalgia::gui::init_gui());
}