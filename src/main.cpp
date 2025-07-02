#include "ui/headless.h"
#include "ui/gui.h"

#include <string>

int main(int argc, char** argv) {
    bool headless = false;

    for (int i = 1; i < argc; ++i) {
        
        if (std::string(argv[i]) == "-headless") {
            headless = true;
        }
    }

    if (headless) {
        return nostalgia::init_headless();
    } else {
        return nostalgia::gui::init_gui();
    }
}