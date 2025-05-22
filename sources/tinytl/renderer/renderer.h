#ifndef RENDERER_H
#define RENDERER_H
#include "renderer_common.h"

       

inline void Renderer_Init()
{
    Renderer_Platform_Init();
}

inline void Renderer_Update()
{
    Renderer_Platform_Update();
}

inline uint8_t Renderer_Get_TargetType()
{
    return Renderer_Platform_Get_TargetType();
}

inline uint8_t Renderer_Get_HardwareBackend()
{
    return Renderer_Platform_Get_HardwareBackend();
}

inline uint8_t Renderer_Get_DataFormat()
{
    return Renderer_Platform_Get_DataFormat();
}

inline void Renderer_Shutdown()
{
   Renderer_Platform_Shutdown();
}


#endif