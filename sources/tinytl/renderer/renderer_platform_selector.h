#ifndef RENDERER_PLATFORM_SELECTOR_H
#define RENDERER_PLATFORM_SELECTOR_H
#include <tinytl/platform_selector.h>

#ifdef PLATFORM_WINDOWS 
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "gdi32.lib")
#elif PLATFORM_MACOS
#elif PLATFORM_LINUX
#else
#error Unsupported platform
#endif



#endif