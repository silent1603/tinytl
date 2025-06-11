#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM
#include "common.h"
#include "window_system.h"
enum WindowPropertiesBitIndex
{
    FULLSCREEN = BIT(0),
    VSYNC = BIT(1)
};

enum MouseStateBitIndex
{
    LDOWN = BIT(0),
    RDOWN = BIT(1)
};
typedef struct
{
#if PLATFORM_WINDOWS
    HINSTANCE hInstance;
#endif
} WindowInitParams;

typedef struct
{
    int oldMousePos[2];
    int currentMousePos[2];
    int windowSize[2];
    int colorBits[4];
    int depthBits;
    int stencilBits;
    int mouseState;
    int properties;
#if PLATFORM_WINDOWS
    HGLRC hRC; // Rendering context
    HDC hDC;   // Device context
    HWND hWnd; // Window handle
#endif
} WindowData;

typedef struct WindowSystemData
{
    WindowData windata[MAX_WINDOW_SUPPORT];
    int samplesSupported[MAX_SAMPLES_SUPPORTED];
    int currentWindowFocus;
    int currentWindowSize;
#if PLATFORM_WINDOWS
    HINSTANCE hInstance; // Handle to window instance
#endif
} WindowSystemData;

extern WindowSystemData windowSystemdata;

#define CURRENT_WINDOW_FOCUS (windowSystemdata.windata[windowSystemdata.currentWindowFocus])
#define CURRENT_WINDOW_SIZE (windowSystemdata.windata[windowSystemdata.currentWindowSize])
#define GET_FOCUS_INDEX (windowSystemdata.currentWindowFocus)
#define GET_SIZE_INDEX (windowSystemdata.currentWindowSize)
#define GET_WINDOW(i) (windowSystemdata.windata[i])

#define RED_COLOR (CURRENT_WINDOW_SIZE.colorBits[0])
#define GREEN_COLOR (CURRENT_WINDOW_SIZE.colorBits[1])
#define BLUE_COLOR (CURRENT_WINDOW_SIZE.colorBits[2])
#define ALPHA_COLOR (CURRENT_WINDOW_SIZE.colorBits[3])

#if PLATFORM_WINDOWS
#define CURRENT_HDC (CURRENT_WINDOW_FOCUS.hDC)
#define CURRENT_HRC (CURRENT_WINDOW_FOCUS.hRC)
#define CURRENT_HWND (CURRENT_WINDOW_FOCUS.hWnd)
#endif

inline bool Window_Init(WindowData data, WindowInitParams initParams)
{
    int result = 0;
    CURRENT_WINDOW_SIZE.colorBits[0] = data.colorBits[0];
    CURRENT_WINDOW_SIZE.colorBits[1] = data.colorBits[1];
    CURRENT_WINDOW_SIZE.colorBits[2] = data.colorBits[2];
    CURRENT_WINDOW_SIZE.colorBits[3] = data.colorBits[3];
    CURRENT_WINDOW_SIZE.windowSize[0] = data.windowSize[0];
    CURRENT_WINDOW_SIZE.windowSize[1] = data.windowSize[1];
    CURRENT_WINDOW_SIZE.stencilBits = data.stencilBits;
    CURRENT_WINDOW_SIZE.depthBits = data.depthBits;
    CURRENT_WINDOW_SIZE.properties = data.properties;

#if PLATFORM_WINDOWS
    bool WGL_ARB_extensions_string_supported = false;
    bool WGL_ARB_pixel_format_supported = false;
    bool WGL_ARB_multisample_supported = false;
    bool WGL_EXT_swap_control_supported = false;

    bool revertToStandard = false;
    const char *wglExtensions = NULL;

    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
    PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;

    windowSystemdata.hInstance = initParams.hInstance;

    // Init dummy window. Small, no fullscreen
    {
        RECT windowRect;
        windowRect.left = 0;
        windowRect.right = 64;
        windowRect.top = 0;
        windowRect.bottom = 64;

        // Window class structure
        WNDCLASS wc;

        // Fill in window class struct
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Style - redraw on move, own DC
        wc.lpfnWndProc = (WNDPROC)WndProc;             // Wndproc handles messages
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = initParams.hInstance;      // Handle to instance
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);   // Load windows logo icon
        wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load standard cursor
        wc.hbrBackground = NULL;                  // No background required
        wc.lpszMenuName = NULL;                   // No Menu
        wc.lpszClassName = TEXT(CLASSNAME);       // class name

        result = !RegisterClass(&wc);
        if (result)
        {
            // Set window style & extended style
            DWORD style, exStyle;
            exStyle = WS_EX_CLIENTEDGE;
            style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE;

            // Adjust the window size so that client area is the size requested
            AdjustWindowRectEx(&windowRect, style, false, exStyle);

            CURRENT_HWND = CreateWindowEx(exStyle,         // window style
                                          TEXT(CLASSNAME), // class name
                                          TEXT(MAIN_WINDOW_TITLE),
                                          WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style, // style
                                          0, 0,                                      // position
                                          windowRect.right - windowRect.left,        // width
                                          windowRect.bottom - windowRect.top,        // height
                                          NULL, NULL,
                                          initParams.hInstance,
                                          NULL);

            result = CURRENT_HWND != NULL;
            if (!result)
            {
                Shutdown();
            }

            // Set up pixel format
            PIXELFORMATDESCRIPTOR pfd;
            memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

            // Fill in fields
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);                                // size
            pfd.nVersion = 1;                                                         // version
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // flags
            pfd.iPixelType = PFD_TYPE_RGBA;                                           // rgba

            pfd.cColorBits = CURRENT_WINDOW_SIZE.colorBits[0] + CURRENT_WINDOW_SIZE.colorBits[1] + CURRENT_WINDOW_SIZE.colorBits[2]; // color bits
            pfd.cRedBits = CURRENT_WINDOW_SIZE.colorBits[0];
            pfd.cGreenBits = CURRENT_WINDOW_SIZE.colorBits[1];
            pfd.cBlueBits = CURRENT_WINDOW_SIZE.colorBits[2];
            pfd.cAlphaBits = CURRENT_WINDOW_SIZE.colorBits[3];

            pfd.cDepthBits = CURRENT_WINDOW_SIZE.depthBits;     // depth bits
            pfd.cStencilBits = CURRENT_WINDOW_SIZE.stencilBits; // stencil bits

            pfd.iLayerType = PFD_MAIN_PLANE;

            // Get DC
            CURRENT_HDC = GetDC(CURRENT_HWND);
            result = CURRENT_HDC != NULL;

            if (result)
            {
                GLuint chosenFormat = ChoosePixelFormat(CURRENT_HDC, &pfd);
                result = chosenFormat != NULL;
                if (result)
                {
                    result = SetPixelFormat(CURRENT_HDC, chosenFormat, &pfd);
                }
                if (result)
                {
                    CURRENT_HRC = wglCreateContext(CURRENT_HDC);
                    result = CURRENT_HRC != NULL;
                }

                result = result ? !wglMakeCurrent(CURRENT_HDC, CURRENT_HRC) : 0;
            }
        }
    }
    // Initiate the WGL extensions. If not supported, use the standard window
    {
        if (result)
        {
            wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
            WGL_ARB_extensions_string_supported = wglGetExtensionsStringARB != NULL;
        }

        if (WGL_ARB_extensions_string_supported)
        {
            wglExtensions = wglGetExtensionsStringARB(CURRENT_HDC);

            char *extensionString = (char *)wglExtensions;
            char *extensionARBPixelFormatName = "WGL_ARB_pixel_format";
            char *extensionEXTSwapName = "WGL_EXT_swap_control";
            char *extensionaARBMultiSampleName = "WGL_ARB_multisample";
            bool WGL_ARB_pixel_format_supported = false;

            while (*extensionString != '\0')
            {
                const char *extStr = extensionString;

                // Check against ARB
                {
                    const char *extName = extensionARBPixelFormatName;
                    const char *cursor = extStr;
                    bool match = true;

                    while (*extName && *cursor && *cursor != ' ')
                    {
                        if (*extName != *cursor)
                        {
                            match = false;
                            break;
                        }
                        ++extName;
                        ++cursor;
                    }

                    if (match && *extName == '\0' && (*cursor == '\0' || *cursor == ' '))
                    {
                        WGL_ARB_pixel_format_supported = true;
                        wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
                            wglGetProcAddress("wglGetPixelFormatAttribivARB");
                        wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)
                            wglGetProcAddress("wglGetPixelFormatAttribfvARB");
                        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)
                            wglGetProcAddress("wglChoosePixelFormatARB");
                    }
                }

                // Check against EXT
                {
                    const char *extName = extensionaARBMultiSampleName;
                    const char *cursor = extStr;
                    bool match = true;

                    while (*extName && *cursor && *cursor != ' ')
                    {
                        if (*extName != *cursor)
                        {
                            match = false;
                            break;
                        }
                        ++extName;
                        ++cursor;
                    }

                    if (match && *extName == '\0' && (*cursor == '\0' || *cursor == ' '))
                    {
                        WGL_EXT_swap_control_supported = true;
                        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)
                            wglGetProcAddress("wglSwapIntervalEXT");
                        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)
                            wglGetProcAddress("wglGetSwapIntervalEXT");
                    }
                }

                {
                    const char *extName = extensionEXTSwapName;
                    const char *cursor = extStr;
                    bool match = true;

                    while (*extName && *cursor && *cursor != ' ')
                    {
                        if (*extName != *cursor)
                        {
                            match = false;
                            break;
                        }
                        ++extName;
                        ++cursor;
                    }

                    if (match && *extName == '\0' && (*cursor == '\0' || *cursor == ' '))
                    {
                        WGL_ARB_multisample_supported = true;
                    }
                }
                // Skip to next extension name
                while (*extensionString != '\0' && *extensionString != ' ')
                    ++extensionString;
                if (*extensionString == ' ')
                    ++extensionString;
            }

            if (!WGL_ARB_pixel_format_supported || !WGL_EXT_swap_control_supported)
            {
                revertToStandard = true;
            }
        }
    }
    // If the necessary extensions are not supported, use the standard window
    if (revertToStandard)
    {
        // shutdown dummy
        // Shut down rendering context
        if (CURRENT_HRC)
        {
            // Stop the context from being current
            wglMakeCurrent(NULL, NULL);
            // Delete the context
            wglDeleteContext(CURRENT_HRC);

            CURRENT_HRC = NULL;
        }

        // Release DC
        if (CURRENT_HDC)
        {
            ReleaseDC(CURRENT_HWND, CURRENT_HDC);
            CURRENT_HDC = NULL;
        }

        // Destroy window
        if (CURRENT_HWND)
        {
            DestroyWindow(CURRENT_HWND);
            CURRENT_HWND = NULL;
        }

        // Unregister class
        UnregisterClass(TEXT(CLASSNAME), windowSystemdata.hInstance);

        // InitStandard
        // Create a rect structure for the size/position of the window
        RECT windowRect;
        windowRect.left = 0;
        windowRect.right = (long)data.windowSize[0];
        windowRect.top = 0;
        windowRect.bottom = (long)data.windowSize[1];

        // Window class structure
        WNDCLASS wc;

        // Fill in window class struct
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Style - redraw on move, own DC
        wc.lpfnWndProc = (WNDPROC)WndProc;             // Wndproc handles messages
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = initParams.hInstance;      // Handle to instance
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);   // Load windows logo icon
        wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load standard cursor
        wc.hbrBackground = NULL;                  // No background required
        wc.lpszMenuName = NULL;                   // No Menu
        wc.lpszClassName = TEXT(CLASSNAME);       // class name

        // Register window class
        result = !RegisterClass(&wc);

        // Switch to fullscreen if required
        if (HAS_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN))
        {
            DEVMODE screenSettings; // device mode
            memset(&screenSettings, 0, sizeof(screenSettings));

            screenSettings.dmSize = sizeof(screenSettings);

            // Set size & color bits
            screenSettings.dmPelsWidth = data.windowSize[0];
            screenSettings.dmPelsHeight = data.windowSize[1];

            screenSettings.dmBitsPerPel = data.colorBits[0] + data.colorBits[1] + data.colorBits[2] + data.colorBits[3];
            screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try to change to full screen
            if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                // If failed, ask whether to run in window
                char *errorText = "The Requested Full Screen Mode Is Not Supported By\n Your Video Card. Use Windowed Mode Instead?";
                int isChangeToWindowMode = MessageBox(NULL, errorText, TEXT(MAIN_WINDOW_TITLE), MB_YESNO | MB_ICONEXCLAMATION);
                if (isChangeToWindowMode == IDYES)
                {
                    UNSET_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN);
                }
                result = isChangeToWindowMode != IDYES;
            }
        }

        if (result)
        {
            // Set window style & extended style
            DWORD style = HAS_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN) ? WS_EX_APPWINDOW : WS_EX_CLIENTEDGE;
            DWORD exStyle = HAS_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN) ? WS_POPUP | WS_VISIBLE : WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE;
            if (HAS_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN))
            {
                // Hide cursor
                ShowCursor(false);
            }

            // Adjust the window size so that client area is the size requested
            AdjustWindowRectEx(&windowRect, style, false, exStyle);

            // Create Window
            CURRENT_HWND = CreateWindowEx(exStyle,         // window style
                                          TEXT(CLASSNAME), // class name
                                          TEXT(MAIN_WINDOW_TITLE),
                                          WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style, // style
                                          0, 0,                                      // position
                                          windowRect.right - windowRect.left,        // width
                                          windowRect.bottom - windowRect.top,        // height
                                          NULL, NULL,
                                          windowSystemdata.hInstance,
                                          NULL);

            result = CURRENT_HWND != NULL;

            if (result)
            {
                // Set up pixel format
                PIXELFORMATDESCRIPTOR pfd;
                memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

                // Fill in fields
                pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);                                // size
                pfd.nVersion = 1;                                                         // version
                pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // flags
                pfd.iPixelType = PFD_TYPE_RGBA;                                           // rgba

                pfd.cColorBits = CURRENT_WINDOW_SIZE.colorBits[0] + CURRENT_WINDOW_SIZE.colorBits[1] + CURRENT_WINDOW_SIZE.colorBits[2]; // color bits
                pfd.cRedBits = CURRENT_WINDOW_SIZE.colorBits[0];
                pfd.cGreenBits = CURRENT_WINDOW_SIZE.colorBits[1];
                pfd.cBlueBits = CURRENT_WINDOW_SIZE.colorBits[2];
                pfd.cAlphaBits = CURRENT_WINDOW_SIZE.colorBits[3];

                pfd.cDepthBits = CURRENT_WINDOW_SIZE.depthBits;     // depth bits
                pfd.cStencilBits = CURRENT_WINDOW_SIZE.stencilBits; // stencil bits

                pfd.iLayerType = PFD_MAIN_PLANE;

                // Get DC
                CURRENT_HDC = GetDC(CURRENT_HWND);
                GLuint chosenFormat;
                // Find a pixel format
                result = CURRENT_HDC != NULL ? chosenFormat = ChoosePixelFormat(CURRENT_HDC, &pfd) : 0;
                result = result ? SetPixelFormat(CURRENT_HDC, chosenFormat, &pfd) : 0;

                if (result)
                {
                    CURRENT_HRC = wglCreateContext(CURRENT_HDC);
                    result = CURRENT_HRC != NULL;
                }

                result = result ? wglMakeCurrent(CURRENT_HDC, CURRENT_HRC) : 0;

                if (result)
                {
                    // Show window
                    ShowWindow(CURRENT_HWND, SW_SHOW);
                    SetForegroundWindow(CURRENT_HWND);
                    SetFocus(CURRENT_HWND);

                    // Update class variables
                    glGetIntegerv(GL_RED_BITS, &CURRENT_WINDOW_SIZE.colorBits[0]);
                    glGetIntegerv(GL_GREEN_BITS, &CURRENT_WINDOW_SIZE.colorBits[1]);
                    glGetIntegerv(GL_BLUE_BITS, &CURRENT_WINDOW_SIZE.colorBits[2]);
                    glGetIntegerv(GL_ALPHA_BITS, &CURRENT_WINDOW_SIZE.colorBits[3]);
                    glGetIntegerv(GL_DEPTH_BITS, &CURRENT_WINDOW_SIZE.depthBits);
                    glGetIntegerv(GL_STENCIL_BITS, &CURRENT_WINDOW_SIZE.stencilBits);
                }
            }
        }
    }

    for (int i = 0; i < MAX_SAMPLES_SUPPORTED; i++)
    {
        windowSystemdata.samplesSupported[i] = false;
    }

    windowSystemdata.samplesSupported[0] = WGL_ARB_multisample_supported ? windowSystemdata.samplesSupported[0] : true;
    if (WGL_ARB_multisample_supported)
    {
        int attributes[12];
        int results[12];

        // Find out how many PFDs there are
        attributes[0] = WGL_NUMBER_PIXEL_FORMATS_ARB;
        wglGetPixelFormatAttribivARB(CURRENT_HDC, 1, 0, 1, attributes, results);
        int numPfds = results[0];

        // A list of attributes to check for each pixel format
        attributes[0] = WGL_RED_BITS_ARB; // bits
        attributes[1] = WGL_GREEN_BITS_ARB;
        attributes[2] = WGL_BLUE_BITS_ARB;
        attributes[3] = WGL_ALPHA_BITS_ARB;
        attributes[4] = WGL_DEPTH_BITS_ARB;
        attributes[5] = WGL_STENCIL_BITS_ARB;

        attributes[6] = WGL_DRAW_TO_WINDOW_ARB; // required to be true
        attributes[7] = WGL_SUPPORT_OPENGL_ARB;
        attributes[8] = WGL_DOUBLE_BUFFER_ARB;

        attributes[9] = WGL_ACCELERATION_ARB; // required to be FULL_ACCELERATION_ARB

        attributes[10] = WGL_SAMPLE_BUFFERS_ARB; // Multisample
        attributes[11] = WGL_SAMPLES_ARB;

        // Loop through all the pixel formats
        for (int i = 0; i < numPfds; ++i)
        {
            // Get the attributes
            wglGetPixelFormatAttribivARB(CURRENT_HDC, i + 1, 0, 12, attributes, results);

            // See if this format supports the bits required
            if (results[0] != CURRENT_WINDOW_SIZE.colorBits[0] || results[1] != CURRENT_WINDOW_SIZE.colorBits[1] ||
                results[2] != CURRENT_WINDOW_SIZE.colorBits[2] || results[3] != CURRENT_WINDOW_SIZE.colorBits[3] ||
                results[4] != CURRENT_WINDOW_SIZE.depthBits || results[5] != CURRENT_WINDOW_SIZE.stencilBits)
                continue;

            // Ensure required attributes are true
            if (results[6] == false || results[7] == false ||
                results[8] == false || results[9] != WGL_FULL_ACCELERATION_ARB)
                continue;

            // Save the number of samples in this pixel format
            if (results[10] == false)
                windowSystemdata.samplesSupported[0] = true;
            if (!windowSystemdata.samplesSupported[0] && results[11] <= 16) // don't support >16x AA
                windowSystemdata.samplesSupported[results[11]] = true;
        }
    }

    if(WGL_ARB_extensions_string_supported)
    {
        // shutdown dummy
        // Shut down rendering context
        if (CURRENT_HRC)
        {
            // Stop the context from being current
            wglMakeCurrent(NULL, NULL);
            // Delete the context
            wglDeleteContext(CURRENT_HRC);

            CURRENT_HRC = NULL;
        }

        // Release DC
        if (CURRENT_HDC)
        {
            ReleaseDC(CURRENT_HWND, CURRENT_HDC);
            CURRENT_HDC = NULL;
        }

        // Destroy window
        if (CURRENT_HWND)
        {
            DestroyWindow(CURRENT_HWND);
            CURRENT_HWND = NULL;
        }

        // Unregister class
        UnregisterClass(TEXT(CLASSNAME), windowSystemdata.hInstance);

        //init Extended
        
    }
#else

#endif
    if (!result)
    {
        Shutdown();
    }
    return result;
}

inline void Window_Update(float dt)
{
}

inline void Window_Shutdown()
{
#if PLATFORM_WINDOWS
    if (HAS_FLAG(CURRENT_WINDOW_FOCUS.properties, FULLSCREEN))
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(true);
    }
    // Shut down rendering context
    if (CURRENT_HRC)
    {
        // Stop the context from being current
        wglMakeCurrent(NULL, NULL);
        // Delete the context
        wglDeleteContext(CURRENT_HRC);
        CURRENT_HRC = NULL;
    }

    // Release DC
    if (CURRENT_HDC)
    {
        ReleaseDC(CURRENT_HWND, CURRENT_HDC);
        CURRENT_HDC = NULL;
    }

    // Destroy window
    if (CURRENT_HWND)
    {
        DestroyWindow(CURRENT_HWND);
        CURRENT_HWND = NULL;
    }

    UnregisterClass(Text(CLASSNAME), windowSystemdata.hInstance);
#endif
}

inline void Window_SaveScreenShot()
{
}

inline void Window_GetCurrentWindowActiveIndex()
{
}
#endif