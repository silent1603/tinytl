#ifndef CONSOLE_INPUT_WIN32_IMPL_H
#define CONSOLE_INPUT_WIN32_IMPL_H
#include "tinytl/io/input_common.h"
#ifdef PLATFORM_WINDOWS 
struct EventIn
{
    INPUT_RECORD eventsRecord[MAX_EVENT_RECEIVED];
    HANDLE hInput; 
    DWORD eventsRead;
    DWORD numEvents ;
};
inline void IO_Console_PollEvents(EventIn& event)
{
    GetNumberOfConsoleInputEvents(event.hInput, &event.numEvents);
    if(event.numEvents > 0)
    {
        ReadConsoleInput(event.hInput, event.eventsRecord, event.numEvents, &event.eventsRead);
    }
}
#endif
#endif