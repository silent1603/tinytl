#ifndef RENDERER_COMMON_H
#define RENDERER_COMMON_H
#include "renderer_platform_selector.h"
#include "renderer_data_format.h"
#include "renderer_hardware_backend.h"
#include "renderer_target_type.h"
#include <tinytl/container/vector.h>
enum RENDERER_PARAMETERS_INDICES 
{
    WINDOW_WIDTH = 0,
    WINDOW_HEIGHT,
    HARDWARE_TYPE,
    TARGET_TYPE,
    DATA_FORMAT,
    FRAMEBUFFER_NUMBER,
    RENDERER_PARAMETERS_INDICES_SIZE
};

struct RendererHandler
{
    void* handler;
};
      
#include "win32/renderer_win32_impl.inl"
#include "osx/renderer_osx_impl.inl"
#include "linux/renderer_linux_impl.inl"

#endif