#ifndef RENDERER_H
#define RENDERER_H
#include "renderer_common.h"

extern Vector<int,RENDERER_PARAMETERS_INDICES::RENDERER_PARAMETERS_INDICES_SIZE> renderer_information;        

inline void Renderer_Init(Vector<int,RENDERER_PARAMETERS_INDICES::RENDERER_PARAMETERS_INDICES_SIZE> renderer_params)
{
    renderer_information = renderer_params;
    Renderer_PlatformInit(renderer_information);
}

inline void Renderer_Update()
{
    Renderer_PlatformUpdate();
}

inline void RetrieveAllConfigs(Vector<int,RENDERER_PARAMETERS_INDICES::RENDERER_PARAMETERS_INDICES_SIZE>& config)
{
    Renderer_PlatformRetrieveConfigs(config);
}

inline void Renderer_Shutdown()
{
   Renderer_PlatformShutdown();
}


#endif