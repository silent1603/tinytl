// #include "tinytl/renderer/renderer.h"
// #include "tinytl/renderer/console_renderer.h"
// #include "tinytl/io/io.h"

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// {
//     ConsoleRendererInitParams inputParams = {};
//     inputParams.fontSize = {MIN_FONT_WIDTH, MIN_FONT_HEIGHT};
//     inputParams.windowSize = {1280, 720};
//     inputParams.cursorInfo.bVisible = false;
//     EventIn events;
//     if (Console_Renderer_Init(inputParams))
//     {

//         events.hInput = GetStdHandle(STD_INPUT_HANDLE);
//         const int width = consoleRendererData.bufferSize.X;
//         const int height = consoleRendererData.bufferSize.Y;

//         // High-resolution timer setup
//         LARGE_INTEGER freq, tStart, tNow;
//         QueryPerformanceFrequency(&freq);
//         QueryPerformanceCounter(&tStart);

//         float posX = 0.0f;
//         const float speed = 40.0f; // cells/sec

//         CHAR_INFO clearChar;
//         clearChar.Char.UnicodeChar = ' ';
//         clearChar.Attributes = BACKGROUND_BLUE;

//         bool running = true;
//         while (running)
//         {
//             // Check input (non-blocking)
//             IO_Console_PollEvents(events);
//             if (events.numEvents > 0)
//             {
//                 for (int i = 0; i < events.numEventsRead; ++i)
//                 {
//                     const auto &record = events.eventsRecord[i];
//                     switch (record.EventType)
//                     {
//                     case KEY_EVENT:
//                     {
//                         const KEY_EVENT_RECORD &key = record.Event.KeyEvent;
//                         if (key.bKeyDown && key.wVirtualKeyCode == VK_ESCAPE)
//                             running = false;
//                         break;
//                     }
//                     case MOUSE_EVENT:
//                         break;
//                     case WINDOW_BUFFER_SIZE_EVENT:
//                     {
//                         const WINDOW_BUFFER_SIZE_RECORD &sizeEvent = record.Event.WindowBufferSizeEvent;
//                         Console_Renderer_OnWindowSizeChanged(sizeEvent.dwSize.X,sizeEvent.dwSize.Y);
//                         break;
//                     }
//                     case FOCUS_EVENT:
//                     {
//                         const FOCUS_EVENT_RECORD &focus = record.Event.FocusEvent;
//                         running = &focus.bSetFocus;
//                         break;
//                     }
//                     }
//                 }
//             }
//             // Delta time
//             QueryPerformanceCounter(&tNow);
//             float deltaTime = float(tNow.QuadPart - tStart.QuadPart) / freq.QuadPart;
//             tStart = tNow;

//             Console_Renderer_Clear(clearChar);

//             // Update position
//             posX += speed * deltaTime;
//             if (posX >= width)
//                 posX = 0;

//             // Draw animated char
//             int x = static_cast<int>(posX);
//             int y = height / 2;
//             int index = y * width + x;
//             consoleRendererData.screenBuffer[index].Char.UnicodeChar = 0x2588;
//             consoleRendererData.screenBuffer[index].Attributes = BACKGROUND_BLUE | FOREGROUND_GREEN;
//             Console_Renderer_Present();
//         }

//         Console_Renderer_Shutdown();
//     }

//     return 0;
// }

#include <vector>
#include "tinytl/memory/allocator/allocator.h"
#include "tinytl/memory/allocator/policy/stardard_alloc_policy.h"
#include "tinytl/memory/allocator/traits/stardard_allocator_traits.h"
#include "tinytl/platform_selector.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    std::vector<int, Allocator<int, alignof(int),StardardAllocatorPolicy<int,alignof(int)>,StardardAllocatorTraits<int>,512>> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }
    v.clear();
    v.shrink_to_fit(); // triggers deallocation
    return 0;
}
