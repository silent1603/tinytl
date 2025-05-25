#ifndef INPUT_PLATFORM_SELECTOR_H
#define INPUT_PLATFORM_SELECTOR_H
#include "tinytl/platform_selector.h"

#ifdef PLATFORM_WINDOWS 
#elif PLATFORM_MACOS
#elif PLATFORM_LINUX
#else
#error Unsupported platform
#endif



#endif