#ifndef CONSOLE_RENDERER_WIN32_IMPL_H
#define CONSOLE_RENDERER_WIN32_IMPL_H
#include "tinytl/common.h"
#ifdef PLATFORM_WINDOWS
#include <strsafe.h>
struct ConsoleRendererData
{
    CHAR_INFO screenBuffer[MAX_CONSOLE_COLS * MAX_CONSOLE_ROWS];
    wchar_t consoleTitleName[SUPPORT_CONSOLE_TITLE_NAME_SIZE];
    SMALL_RECT writeRegion;
    HANDLE consoleBufferHandler;
    HANDLE consoleInputHandler;
    HANDLE consoleOutputHandler;
    DWORD consoleIOMode[2];            // input , output
    CONSOLE_CURSOR_INFO cursorInfo[2]; // old , current
    COORD bufferSize = {0, 0};
    COORD bufferCoord = {0, 0};
    Vector<int, 2> maxWindowPixelSize;
    Vector<int, 2> currentWindowPixelSize;
    Vector<int, 2> fontSize;
};

void PrintLastError(const wchar_t *context)
{
    DWORD errorCode = GetLastError();
    LPWSTR messageBuffer = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        NULL);

    if (messageBuffer)
    {
        wchar_t fullMessage[512];
        StringCchPrintfW(fullMessage, 512, L"%s failed with error %lu: %s", context, errorCode, messageBuffer);
        OutputDebugStringW(fullMessage);
        LocalFree(messageBuffer);
    }
}

struct ConsoleRendererInitParams
{
    Vector<int, 2> windowSize = {0};
    Vector<int, 2> fontSize = {MIN_FONT_WIDTH, MIN_FONT_HEIGHT}; // width , height;
    CONSOLE_CURSOR_INFO cursorInfo;
    DWORD consoleIOMode[2] = {ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT, ENABLE_VIRTUAL_TERMINAL_PROCESSING};
};

ConsoleRendererData consoleRendererData;

inline bool Console_Renderer_Init(ConsoleRendererInitParams inputParams)
{
    bool result = 1;
    if (GetConsoleWindow() == NULL)
    {
        result = AllocConsole();
    }
    if (result)
    {
        HANDLE hBufferConsoleHandler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        result = hBufferConsoleHandler != INVALID_HANDLE_VALUE;
        result ? void() : OutputDebugString(TEXT("Failed to create screen buffer."));
        result = SetConsoleActiveScreenBuffer(hBufferConsoleHandler);

        if (result)
        {
            consoleRendererData.consoleBufferHandler = hBufferConsoleHandler;

            HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
            HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

            consoleRendererData.consoleInputHandler = hConsoleInput;
            consoleRendererData.consoleOutputHandler = hConsoleOutput;

            SetConsoleMode(hConsoleInput, inputParams.consoleIOMode[0]);
            SetConsoleMode(hConsoleOutput, inputParams.consoleIOMode[1]);

            consoleRendererData.consoleIOMode[0] = inputParams.consoleIOMode[0];
            consoleRendererData.consoleIOMode[1] = inputParams.consoleIOMode[1];
            consoleRendererData.maxWindowPixelSize.data[0] = GetSystemMetrics(SM_CXSCREEN);
            consoleRendererData.maxWindowPixelSize.data[1] = GetSystemMetrics(SM_CYSCREEN);

            consoleRendererData.currentWindowPixelSize.data[0] = consoleRendererData.maxWindowPixelSize.data[0] > inputParams.windowSize.data[0] ? inputParams.windowSize.data[0] : consoleRendererData.maxWindowPixelSize.data[0];
            consoleRendererData.currentWindowPixelSize.data[1] = consoleRendererData.maxWindowPixelSize.data[1] > inputParams.windowSize.data[1] ? inputParams.windowSize.data[1] : consoleRendererData.maxWindowPixelSize.data[1];

            HWND hWnd = GetConsoleWindow();
            RECT rect = {0, 0, consoleRendererData.currentWindowPixelSize.data[0], consoleRendererData.currentWindowPixelSize.data[1]};
            result = SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
            if (result)
            {
                CONSOLE_FONT_INFOEX cfi = {sizeof(CONSOLE_FONT_INFOEX)};
                consoleRendererData.fontSize.data[0] = inputParams.fontSize.data[0];
                consoleRendererData.fontSize.data[1] = inputParams.fontSize.data[1];
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

                SetCurrentConsoleFontEx(hBufferConsoleHandler, FALSE, &cfi);

                // Set buffer size
                short preferedCols = consoleRendererData.currentWindowPixelSize.data[0] / consoleRendererData.fontSize.data[0];
                short preferedRows = consoleRendererData.currentWindowPixelSize.data[1] / consoleRendererData.fontSize.data[1];
                short consoleCols = MAX_CONSOLE_COLS > preferedCols ? preferedCols : MAX_CONSOLE_COLS;
                short consoleRows = MAX_CONSOLE_ROWS > preferedRows ? preferedRows : MAX_CONSOLE_ROWS;
                consoleRendererData.bufferSize.X = consoleCols;
                consoleRendererData.bufferSize.Y = consoleRows;
                SetConsoleScreenBufferSize(hBufferConsoleHandler, consoleRendererData.bufferSize);

                const wchar_t *titleName = L"Game";
                for (int i = 0; i < SUPPORT_CONSOLE_TITLE_NAME_SIZE; ++i)
                {
                    consoleRendererData.consoleTitleName[i] = titleName[i];
                    if (titleName[i] == L'\0')
                        break;
                }

                SetConsoleTitle(consoleRendererData.consoleTitleName);

                GetConsoleCursorInfo(hBufferConsoleHandler, &consoleRendererData.cursorInfo[0]);
                PrintLastError(L"GetConsoleCursorInfo");
                const CONSOLE_CURSOR_INFO cursorInfo = inputParams.cursorInfo;
                consoleRendererData.cursorInfo[1] = cursorInfo;
                SetConsoleCursorInfo(hBufferConsoleHandler, &cursorInfo);
                PrintLastError(L"SetConsoleCursorInfo");
                consoleRendererData.writeRegion = {0, 0, SHORT(consoleRendererData.bufferSize.X - 1), SHORT(consoleRendererData.bufferSize.Y - 1)};
                SetConsoleWindowInfo(hBufferConsoleHandler, TRUE, &consoleRendererData.writeRegion);
                PrintLastError(L"SetConsoleWindowInfo");
            }
        }
    }

    return result;
}

inline void Console_Renderer_Present()
{
    WriteConsoleOutput(consoleRendererData.consoleBufferHandler,
                       consoleRendererData.screenBuffer,
                       consoleRendererData.bufferSize,
                       consoleRendererData.bufferCoord,
                       &consoleRendererData.writeRegion);
}

inline void Console_Renderer_OnWindowSizeChanged(SHORT width, SHORT row)
{
    consoleRendererData.bufferSize.X = width;
    consoleRendererData.bufferSize.Y = row;
    consoleRendererData.writeRegion.Right = SHORT(consoleRendererData.bufferSize.X - 1);
    consoleRendererData.writeRegion.Bottom = SHORT(consoleRendererData.bufferSize.Y - 1);
    consoleRendererData.currentWindowPixelSize.data[0] = int(width) * consoleRendererData.fontSize.data[0] ;
    consoleRendererData.currentWindowPixelSize.data[1] = int(row) * consoleRendererData.fontSize.data[1];

    SetConsoleWindowInfo(consoleRendererData.consoleBufferHandler, TRUE, &consoleRendererData.writeRegion);
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
    CloseHandle(consoleRendererData.consoleBufferHandler);
}

inline void Console_Renderer_Enable_Cursor(bool enable)
{
    consoleRendererData.cursorInfo[1].bVisible = enable;
}

#endif
#endif