#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM
#include "common.h"
#include "window_system.h"

// Window data structure
typedef struct {
#ifdef PLATFORM_WINDOWS
    HWND hwnd;
    HDC hdc;
#endif
    int numSamples[16];
    int screen[2];
    int colorBits[4];
    int depthBits;
    int stencilBits;
    int properties;
    EWindowTag tag;
} WindowData;

typedef struct
{
#ifdef PLATFORM_WINDOWS
    HINSTANCE hInstance;
#endif

} WindowSystemInitParams;

typedef struct
{
#ifdef PLATFORM_WINDOWS
    HINSTANCE hInstance;
#endif
    WindowData windows[MAX_WINDOW_SUPPORTED];
    int currentWindowSize;
    int currentWindowFocus;
    int properties;
} WindowSystemData;

#define CURRENT_WINDOW_SIZE windowSystemData.windows[windowSystemData.currentWindowSize] 
#define CURRENT_WINDOW_FOCUS windowSystemData.windows[windowSystemData.currentWindowFocus] 

#ifdef PLATFORM_WINDOWS
#define CURRENT_WINDOW_FOCUS_HWND windowSystemData.windows[windowSystemData.currentWindowFocus].hwnd
#define CURRENT_WINDOW_FOCUS_HDC windowSystemData.windows[windowSystemData.currentWindowFocus].hdc
#endif 

inline int WindowSystem_Init(WindowSystemInitParams params)
{
#ifdef PLATFORM_WINDOWS
    windowSystemData.hInstance = params.hInstance;
#endif
    return true;
}

inline int Window_Create(WindowData data)
{
    int result = 0;
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = windowSystemData.hInstance;
    wc.lpszClassName = WindowNameCollections[MAIN_MENU];
    result = RegisterClass(&wc);
    if (result)
    {
        CURRENT_WINDOW_FOCUS_HWND = CreateWindowEx(WindowNameCollections[MAIN_MENU], WindowNameCollections[MAIN_MENU], WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, CURRENT_WINDOW_FOCUS.screen[0], CURRENT_WINDOW_FOCUS.screen[1],
                                NULL, NULL, windowSystemData.hInstance, NULL);
    if (!window->hwnd) {
        fprintf(stderr, "Failed to create window\n");
        return 0;
    }

    window->hdc = GetDC(window->hwnd);
    if (!window->hdc) {
        DestroyWindow(window->hwnd);
        return 0;
    }

        ShowWindow(window->hwnd, SW_SHOW);

    }

    return result;
}

inline void Window_Destroy()
{
    if (CURRENT_WINDOW_FOCUS_HDC)
        ReleaseDC(CURRENT_WINDOW_FOCUS_HWND, CURRENT_WINDOW_FOCUS_HDC);
    if (CURRENT_WINDOW_FOCUS_HWND)
        DestroyWindow(CURRENT_WINDOW_FOCUS_HWND);
    CURRENT_WINDOW_FOCUS_HDC = NULL;
    CURRENT_WINDOW_FOCUS_HWND = NULL;
}

inline bool PollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

inline void Window_SwapBuffers()
{
    #ifdef PLATFORM_WINDOWS
        SwapBuffers(CURRENT_WINDOW_FOCUS_HDC);	
    #endif
}

inline void *Window_GetNativeHandle()
{
    return CURRENT_WINDOW_FOCUS_HDC;
}

inline void Window_SaveScreenShot()
{
}

inline int Window_GetCurrentWindowActiveIdx()
{
    return windowSystemData.currentWindowFocus;
}

inline int Window_GetTotalWindow()
{
    return windowSystemData.currentWindowSize;
}

inline void Window_SetFocusWindow(int index)
{
    windowSystemData.currentWindowFocus = index;
}

inline int Window_GetWindowByTag(EWindowTag tag)
{
    int resultTag = -1;
    for(int i = 0 ; i < MAX_WINDOW_SUPPORTED; ++i)
    {
        int currentWindowTag = windowSystemData.windows[i].tag;
        resultTag = currentWindowTag == tag ? currentWindowTag : resultTag;
    }
    return resultTag;
}

#endif