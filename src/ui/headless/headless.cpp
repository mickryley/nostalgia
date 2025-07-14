#include "headless.h"
#include "utils/log.h"

namespace nostalgia::headless {
   ReturnCode init_headless(const int argc, char** argv){
    (void)argc; (void)argv; 
        log::print(LogFlags::Info, "Nostalgia Headless Mode Initialised");
        
        return ReturnCode::InitHeadlessUnimplementedError;
    }
}