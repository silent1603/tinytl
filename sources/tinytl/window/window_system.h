#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM
#include "common.h"
#include "window_system.h"


// define scene
#define WINDOW_TAG_LIST \
    X(CLASS_NAME,"TinyRenderer") \
    X(MAIN_MENU,"Main Window") \
    X(DUMMY_WINDOW,"Dummy Window") 

enum  { 
    #define X(tag,name) tag, 
    WINDOW_TAG_LIST 
    MAX_WINDOW_SUPPORTED 
    #undef X 
} EWindowTag;

const char* WindowName[] =
{
    #define X(tag,name) name,
    WINDOW_TAG_LIST
    #undef X
};

// Window data structure
typedef struct {
#if PLATFORM_WINDOWS
    HWND hwnd;
    HDC hdc;
#endif
    int numSamples[16];
    int screen[2];
    int colorBits[4];
    int depthBits;
    int stencilBits;
    int properties;
    enum EWindowTag tag;
} WindowData;

typedef struct
{
#if PLATFORM_WINDOWS
    HINSTANCE hInstance;
#endif

} WindowSystemInitParams;

typedef struct
{
#if PLATFORM_WINDOWS
    HINSTANCE hInstance;
#endif
    WindowData windows[MAX_WINDOW_SUPPORTED];
    int currentWindowSize;
    int currentWindowFocus;
    int properties;
} WindowSystemData;

#define CURRENT_WINDOW_SIZE windowSystemData.windows[windowSystemData.currentWindowSize] 
#define CURRENT_WINDOW_FOCUS windowSystemData.windows[windowSystemData.currentWindowFocus] 

#if PLATFORM_WINDOWS
#define CURRENT_WINDOW_FOCUS_HWND windowSystemData.windows[windowSystemData.currentWindowFocus].hwnd
#define CURRENT_WINDOW_FOCUS_HDC windowSystemData.windows[windowSystemData.currentWindowFocus].hdc
#endif 


// --------------------------------- WINDOW SUPPORT FUNCTION -------------------------------------- //
#if PLATFORM_WINDOWS
HWND WindowSystem_CreateDummyWindow() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WindowName[DUMMY_WINDOW];
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(WindowName[DUMMY_WINDOW], NULL, 0, 0, 0, 1, 1,
                             NULL, NULL, GetModuleHandle(NULL), NULL);
    return hwnd;
}

LRESULT CALLBACK DummyWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CREATE:
			return 0;
		case WM_DESTROY:
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


#endif
// --------------------------------- LINUX SUPPORT FUNCTION -------------------------------------- //

inline int WindowSystem_Init(WindowSystemInitParams params)
{
#if PLATFORM_WINDOWS
    windowSystemData.hInstance = params.hInstance;
#endif
    return true;
}

inline int Window_Create(WindowData data)
{
    int result = 0;
#if PLATFORM_WINDOWS

#endif

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
    #if PLATFORM_WINDOWS
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

inline int Window_GetWindowByTag(enum EWindowTag tag)
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