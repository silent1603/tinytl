#ifndef CONSOLE_RENDERER_WIN32_IMPL_H
#define CONSOLE_RENDERER_WIN32_IMPL_H
#include "tinytl/renderer/renderer_common.h"
#ifdef PLATFORM_WINDOWS

struct ConsoleRendererData
{
    CHAR_INFO screenBuffer[MAX_CONSOLE_COLS * MAX_CONSOLE_ROWS];
    wchar_t consoleTitleName[SUPPORT_CONSOLE_TITLE_NAME_SIZE];
    SMALL_RECT windowTerminalSize;
    HANDLE consoleHandler;
    DWORD dwBytesWritten;
    DWORD consoleMode[2];              // old ,current
    CONSOLE_CURSOR_INFO cursorInfo[2]; // old , current
    COORD bufferSize = {0, 0};
    COORD bufferCoord = {0, 0};
    Vector<int, 2> maxWindowPixelSize;
    Vector<int, 2> currentWindowPixelSize;
    Vector<int, 2> fontSize;
};

struct ConsoleRendererInitParams
{
    Vector<int, 2> windowSize = {0};
    Vector<int, 2> fontSize = {MIN_FONT_WIDTH, MIN_FONT_HEIGHT}; // width , height;
    CONSOLE_CURSOR_INFO cursorInfo;
    DWORD consoleMode = ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
};
ConsoleRendererData consoleRendererData;

inline bool Console_Renderer_Init(ConsoleRendererInitParams inputParams)
{
    bool result = 0;
    HANDLE hConsoleHandler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    result = hConsoleHandler != INVALID_HANDLE_VALUE;
    result ? void() : OutputDebugString(TEXT("Failed to create screen buffer."));
    result = SetConsoleActiveScreenBuffer(hConsoleHandler);
    if (result)
    {
        consoleRendererData.consoleHandler = hConsoleHandler;
        
        GetConsoleMode(hConsoleHandler, &consoleRendererData.consoleMode[0]);
        consoleRendererData.consoleMode[1] = inputParams.consoleMode;
        SetConsoleMode(hConsoleHandler, inputParams.consoleMode);

        CONSOLE_FONT_INFOEX cfi = {sizeof(CONSOLE_FONT_INFOEX)};
        cfi.dwFontSize.X = consoleRendererData.fontSize.data[0] == 0 ? MIN_FONT_WIDTH : consoleRendererData.fontSize.data[0];
        cfi.dwFontSize.Y = consoleRendererData.fontSize.data[1] == 0 ? MIN_FONT_HEIGHT : consoleRendererData.fontSize.data[1];
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        const wchar_t *fontName = L"Consolas";

        for (int i = 0; i < LF_FACESIZE; ++i)
        {
            cfi.FaceName[i] = fontName[i];
            if (fontName[i] == L'\0')
                break;
        }

        SetCurrentConsoleFontEx(hConsoleHandler, FALSE, &cfi);

        int maxWindowWidth = GetSystemMetrics(SM_CXSCREEN);
        int maxWindowHeight = GetSystemMetrics(SM_CYSCREEN);
        consoleRendererData.maxWindowPixelSize.data[0] = maxWindowWidth;
        consoleRendererData.maxWindowPixelSize.data[1] = maxWindowHeight;

        int preferedWindowWidth = inputParams.windowSize.data[0] > maxWindowWidth ? maxWindowWidth : inputParams.windowSize.data[0];
        consoleRendererData.currentWindowPixelSize.data[0] = preferedWindowWidth;

        int preferedWindowHeight = inputParams.windowSize.data[1] > maxWindowHeight ? maxWindowHeight : inputParams.windowSize.data[1];
        consoleRendererData.currentWindowPixelSize.data[1] = preferedWindowHeight;

        consoleRendererData.fontSize.data[0] = inputParams.fontSize.data[0];
        consoleRendererData.fontSize.data[1] = inputParams.fontSize.data[1];

        consoleRendererData.bufferSize.X = SHORT(preferedWindowWidth / inputParams.fontSize.data[0]);
        consoleRendererData.bufferSize.Y = SHORT(preferedWindowHeight / inputParams.fontSize.data[1]);
        SetConsoleScreenBufferSize(hConsoleHandler, consoleRendererData.bufferSize);

        consoleRendererData.windowTerminalSize = {0, 0, SHORT(consoleRendererData.bufferSize.X - 1), SHORT(consoleRendererData.bufferSize.Y - 1)};
        SetConsoleWindowInfo(hConsoleHandler, TRUE, &consoleRendererData.windowTerminalSize);

        const wchar_t *titleName = L"Game";
        for (int i = 0; i < SUPPORT_CONSOLE_TITLE_NAME_SIZE; ++i)
        {
            consoleRendererData.consoleTitleName[i] = titleName[i];
            if (titleName[i] == L'\0')
                break;
        }

        SetConsoleTitle(consoleRendererData.consoleTitleName);

        GetConsoleCursorInfo(hConsoleHandler, &consoleRendererData.cursorInfo[0]);
        const CONSOLE_CURSOR_INFO cursorInfo = inputParams.cursorInfo;
        consoleRendererData.cursorInfo[1] = cursorInfo;
        SetConsoleCursorInfo(hConsoleHandler, &cursorInfo);

        COORD cursorPos = {0, 0};
        cursorPos.X = cursorInfo.bVisible ? cursorPos.X : 1000;
        cursorPos.Y = cursorInfo.bVisible ? cursorPos.Y : 1000;
        SetConsoleCursorPosition(hConsoleHandler, cursorPos);
    }

    return result;
}

inline void Console_Renderer_Present()
{
    WriteConsoleOutput(consoleRendererData.consoleHandler, 
        consoleRendererData.screenBuffer, 
        consoleRendererData.bufferSize, 
        consoleRendererData.bufferCoord, 
        &consoleRendererData.windowTerminalSize);
}

inline void Console_Renderer_DrawLine()
{
}

inline void Console_Renderer_Clear(CHAR_INFO info)
{
    int size = consoleRendererData.bufferSize.X * consoleRendererData.bufferSize.Y;
    for (int i = 0; i < size; ++i)
    {
        consoleRendererData.screenBuffer[i].Char.UnicodeChar = info.Char.UnicodeChar;
        consoleRendererData.screenBuffer[i].Attributes = info.Attributes;
    }
}

inline void Console_Renderer_Shutdown()
{
    HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleActiveScreenBuffer(stdOut);
    CloseHandle(consoleRendererData.consoleHandler);
}

inline void Console_Renderer_Enable_Cursor(bool enable)
{
    consoleRendererData.cursorInfo[1].bVisible = enable;
}

#endif
#endif