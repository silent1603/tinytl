#ifndef GL_CONTEXT
#define GL_CONTEXT
#include "tinytl/common.h"

// GLContext data structure
typedef struct {
#ifdef _WIN32
    HGLRC hglrc;
#elif 
#endif
} GLContext;

inline void Renderer_Context_Create(int index)
{

}

inline void Renderer_Context_Destroy(int index)
{

}

inline void Renderer_Context_MakeCurrent(int index)
{

}

#endif