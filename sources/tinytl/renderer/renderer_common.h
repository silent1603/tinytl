#ifndef RENDERER_COMMON_H
#define RENDERER_COMMON_H
#include "renderer_platform_selector.h"
#include "renderer_data_format.h"
#include "renderer_hardware_backend.h"
#include "renderer_target_type.h"
#include <tinytl/container/vector.h>


struct RendererInformation
{
    void* handler;
    Vector<int,2> windowSize;
    int  currentActiveHardwareType;
    int  currentActiveTargetType;
    int  currentActivedataFormatType;
    RendererAvailableTypes availableTypes;
    int  framebufferNumber;
};
struct RendererAvailableTypes {
    uint8_t hardwareTypesState;
    uint8_t targetTypesState;
    uint8_t dataFormatTypesState;
};

struct RendererInitializationParams
{
    void* handler;
    Vector<int,2> windowSize;
    int  hardwareType;
    int  targetType;
    int  dataFormatType;
    int  framebufferNumber;
};
      
#include "win32/renderer_win32_impl.inl"
#include "osx/renderer_osx_impl.inl"
#include "linux/renderer_linux_impl.inl"

#endif