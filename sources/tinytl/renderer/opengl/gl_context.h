#ifndef GL_CONTEXT
#define GL_CONTEXT


// GLContext data structure
typedef struct {
#ifdef _WIN32
    HGLRC hglrc;
#elif 
    GLXContext glx_context;
    Display* display; // Needed for GLX operations
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