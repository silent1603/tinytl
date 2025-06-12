#ifndef INPUT_WIN32_H
#define INPUT_WIN32_H
#include "common.h"
#if PLATFORM_WINDOWS
LRESULT CALLBACK WindowProc(HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch (msg)
	{
	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:	  // Is the screensaver trying to start?
		case SC_MONITORPOWER: // Is the monitor trying to turn off?
			return 0;		  // prevent from happening
		}

		break;
	}

	case WM_CLOSE: // If we are trying to close the window, post a quit message
		PostQuitMessage(0);
		return 0;
		break;
	}

	// Pass all unhandled messages to DefWindowProc
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
#endif