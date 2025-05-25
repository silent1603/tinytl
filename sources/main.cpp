#include "tinytl/renderer/renderer.h"
#include "tinytl/renderer/console_renderer.h"
#include "tinytl/io/io.h"

int main(int argc, char *argv[])
{
    ConsoleRendererInitParams inputParams = {};
    inputParams.fontSize = {MIN_FONT_WIDTH, MIN_FONT_HEIGHT};
    inputParams.windowSize = {1280, 720};
    inputParams.cursorInfo.bVisible = false;
    EventIn events;
    if (Console_Renderer_Init(inputParams))
    {

        events.hInput = GetStdHandle(STD_INPUT_HANDLE);
        const int width = consoleRendererData.bufferSize.X;
        const int height = consoleRendererData.bufferSize.Y;

        // High-resolution timer setup
        LARGE_INTEGER freq, tStart, tNow;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&tStart);

        float posX = 0.0f;
        const float speed = 40.0f; // cells/sec

        CHAR_INFO clearChar;
        clearChar.Char.UnicodeChar = ' ';
        clearChar.Attributes = BACKGROUND_BLUE;

        bool running = true;
        while (running)
        {
            // Check input (non-blocking)
            IO_Console_PollEvents(events);
            if (events.numEvents > 0)
            {
                for (int i = 0; i < events.numEventsRead; ++i)
                {
                    const auto &record = events.eventsRecord[i];
                    switch (record.EventType)
                    {
                    case KEY_EVENT:
                    {
                        const KEY_EVENT_RECORD &key = record.Event.KeyEvent;
                        if (key.bKeyDown && key.wVirtualKeyCode == VK_ESCAPE)
                            running = false;
                        break;
                    }
                    case MOUSE_EVENT:
                        break;
                    case WINDOW_BUFFER_SIZE_EVENT:
                        break;
                    case FOCUS_EVENT:
                        break;
                    }
                }
            }
            // Delta time
            QueryPerformanceCounter(&tNow);
            float deltaTime = float(tNow.QuadPart - tStart.QuadPart) / freq.QuadPart;
            tStart = tNow;

            Console_Renderer_Clear(clearChar);


            consoleRendererData.screenBuffer[0].Char.UnicodeChar = L'X';
            consoleRendererData.screenBuffer[0].Attributes = BACKGROUND_BLUE | FOREGROUND_GREEN;
         
            Console_Renderer_Present();
        }

        Console_Renderer_Shutdown();
    }

    return 0;
}