#ifndef RENDERER_WIN32_IMPL_INL
#define RENDERER_WIN32_IMPL_INL
#include <tinytl/renderer/renderer_common.h>
#ifdef PLATFORM_WINDOWS 
void Renderer_Platform_Init() 
{

}

void Renderer_Platform_Update()
{
    
}

void Renderer_Platform_Shutdown() 
{
   
}

inline uint8_t Renderer_Platform_Get_TargetType()
{
    return 0;
}

inline uint8_t Renderer_Platform_Get_HardwareBackend()
{
    return 0;
}

inline uint8_t Renderer_Platform_Get_DataFormat()
{
    return 0;
}
#endif
#endif