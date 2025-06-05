#ifndef PLATFORM_H
#define PLATOFRM_H
#include "common.h"
#include "config.h"
#if SUPPORT_UTF8
#endif

# if defined(_WIN32)
#  define PLATFORM_WINDOWS 1
# elif defined(__gnu_linux__)
#  define PLATFORM_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define PLATFORM_MACOS 1
# elif defined( __ANDROID__)
#  define PLATFORM_ANDROID 1
# elif defined(__EMSCRIPTEN__)
#  define PLATFORM_WEBASM 1
# else
#  error missing OS detection
# endif

#if PLATFORM_WINDOWS
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include <windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "Shlwapi.lib")

#if MODULE_RENDERER
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "gdi32.lib")
#endif

#endif

#endif