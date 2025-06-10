#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM
#include "platform_selector.h"
#include "window_data.h"

typedef struct WindowSystemData
{
    WindowData windata[MAX_WINDOW_SUPPORT];
    int currentWindowFocus;
    int currentWindowSize;
} WindowSystemData;

extern WindowSystemData windowSystemdata;

inline void Init(WindowData data)
{
#if PLATFORM_WINDOWS
    
#else
    
#endif
}

inline void Update(float dt)
{
}

inline void Shutdown()
{
}

inline void SaveScreenShot()
{
}

inline void GetCurrentWindowActiveIndex()
{
}
#endif