#ifndef WINDOW_DATA_WIN32_H
#define WINDOW_DATA_WIN32_H

#include "tinytl/platform_selector.h"
#if PLATFORM_WINDOWS
typedef struct 
{
	HGLRC		hRC;			//Rendering context
	HDC			hDC;			//Device context
	HWND		hWnd;			//Window handle
	HINSTANCE	hInstance;		//Handle to window instance
} WindowNativeData;

#endif
#endif