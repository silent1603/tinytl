#ifndef CONSOLE_RENDERER_WIN32_IMPL_H
#define CONSOLE_RENDERER_WIN32_IMPL_H
#include "tinytl/renderer/renderer_common.h"
#ifdef PLATFORM_WINDOWS 

struct ConsoleRendererData
{
    CHAR_INFO screenBuffer[MAX_CONSOLE_COLS * MAX_CONSOLE_ROWS];
    wchar_t consoleTitleName[SUPPORT_CONSOLE_TITLE_NAME_SIZE];
    SMALL_RECT writeRegion;
    HANDLE consoleHandler;
    DWORD dwBytesWritten;
    DWORD consoleMode[2]; //old ,current
    CONSOLE_CURSOR_INFO cursorInfo[2]; //old , current
    Vector<short,2> screenColumnRow;
    COORD bufferSize;
    COORD bufferCoord;
    Vector<int,2> maxWindowSize;
    Vector<int,2> currentWindowSize;
    Vector<int,2> fontSize;
};

struct ConsoleRendererInitParams
{
    Vector<int,2> windowSize = {0};
    Vector<int,2> fontSize = {0}; //width , height;
    CONSOLE_CURSOR_INFO cursorInfo;
    DWORD consoleMode = ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING |  ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
};
ConsoleRendererData consoleRendererData;

inline bool Console_Renderer_Init(ConsoleRendererInitParams inputParams) 
{
    bool result = 0;
    HANDLE hConsoleHandler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    result = hConsoleHandler != INVALID_HANDLE_VALUE;
    result ? void() : OutputDebugString(TEXT("Failed to create screen buffer."));
    SetConsoleActiveScreenBuffer(hConsoleHandler);

    consoleRendererData.consoleHandler = hConsoleHandler;

    GetConsoleMode(hConsoleHandler, &consoleRendererData.consoleMode[0]);
    consoleRendererData.consoleMode[1] = inputParams.consoleMode;
    SetConsoleMode(hConsoleHandler,consoleRendererData.consoleMode[1]);
    

    consoleRendererData.maxWindowSize.data[0] =  GetSystemMetrics(SM_CXSCREEN);
    consoleRendererData.maxWindowSize.data[1] =  GetSystemMetrics(SM_CYSCREEN);

    consoleRendererData.currentWindowSize.data[0] =  inputParams.windowSize.data[0];
    consoleRendererData.currentWindowSize.data[1] =  inputParams.windowSize.data[1];

    consoleRendererData.fontSize.data[0] =  inputParams.fontSize.data[0];
    consoleRendererData.fontSize.data[1] =  inputParams.fontSize.data[1];
    

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleHandler, &csbi);
    short consoleCols = MAX_CONSOLE_COLS > csbi.dwMaximumWindowSize.X ?   csbi.dwMaximumWindowSize.X : MAX_CONSOLE_COLS;
    short consoleRows = MAX_CONSOLE_ROWS > csbi.dwMaximumWindowSize.Y ?  csbi.dwMaximumWindowSize.Y :  MAX_CONSOLE_ROWS;
     
    consoleRendererData.screenColumnRow.data[0] = consoleCols; 
    consoleRendererData.screenColumnRow.data[1] = consoleRows;
    
    CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
    cfi.dwFontSize.X = consoleRendererData.fontSize.data[0] == 0 ? MIN_FONT_WIDTH : consoleRendererData.fontSize.data[0];
    cfi.dwFontSize.Y = consoleRendererData.fontSize.data[1] == 0 ? MIN_FONT_HEIGHT : consoleRendererData.fontSize.data[1];
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    const wchar_t* fontName = L"Consolas";
    for (int i = 0; i < LF_FACESIZE; ++i)
    {
        cfi.FaceName[i] = fontName[i];
        if (fontName[i] == L'\0')
            break;
    }

    SetCurrentConsoleFontEx(hConsoleHandler, FALSE, &cfi);
  
    const wchar_t* titleName = L"Game";
    for (int i = 0; i < SUPPORT_CONSOLE_TITLE_NAME_SIZE; ++i)
    {
        consoleRendererData.consoleTitleName[i] = titleName[i];
        if (titleName[i] == L'\0')
            break;
    }

    SetConsoleTitle(consoleRendererData.consoleTitleName);

    // Set buffer size
    consoleRendererData.bufferSize.X = consoleRendererData.screenColumnRow.data[0];
    consoleRendererData.bufferSize.Y =  consoleRendererData.screenColumnRow.data[1];
    SetConsoleScreenBufferSize(hConsoleHandler, consoleRendererData.bufferSize);

    GetConsoleCursorInfo(hConsoleHandler,&consoleRendererData.cursorInfo[0]);
    const CONSOLE_CURSOR_INFO cursorInfo =  inputParams.cursorInfo;
    consoleRendererData.cursorInfo[1] = cursorInfo;
    SetConsoleCursorInfo(hConsoleHandler,&cursorInfo);
    
    COORD cursorPos =  {0,0} ;
    cursorPos.X =  cursorInfo.bVisible ? cursorPos.X :  1000;
    cursorPos.Y =   cursorInfo.bVisible ? cursorPos.Y :  1000;
    SetConsoleCursorPosition(hConsoleHandler, cursorPos);
    
    // Set visible window to match buffer size
    consoleRendererData.writeRegion = { 0, 0, SHORT( consoleRendererData.screenColumnRow.data[0]  - 1),  SHORT(consoleRendererData.screenColumnRow.data[1] - 1 )};
    SetConsoleWindowInfo(hConsoleHandler, TRUE, &consoleRendererData.writeRegion);
    
    return result;
}

inline void Console_Renderer_Present()
{
    WriteConsoleOutput(consoleRendererData.consoleHandler, consoleRendererData.screenBuffer, consoleRendererData.bufferSize, consoleRendererData.bufferCoord, &consoleRendererData.writeRegion);
}

inline void Console_Renderer_DrawLine()
{
    
}

inline void Console_Renderer_Clear(CHAR_INFO info)
{
    int size = consoleRendererData.screenColumnRow.data[0] * consoleRendererData.screenColumnRow.data[1];
    for (int i = 0; i < size; ++i) 
    {
        consoleRendererData.screenBuffer[i].Char.UnicodeChar = info.Char.UnicodeChar;
        consoleRendererData.screenBuffer[i].Attributes = info.Attributes;
    }
}

inline void Console_Renderer_Shutdown() 
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleActiveScreenBuffer(hStdOut);
    CloseHandle(consoleRendererData.consoleHandler);
}

inline void Console_Renderer_Enable_Cursor(bool enable)
{
    consoleRendererData.cursorInfo[1].bVisible = enable;
}



#endif
#endif