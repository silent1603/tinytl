#ifndef CONSOLE_INPUT_WIN32_IMPL_H
#define CONSOLE_INPUT_WIN32_IMPL_H
#include "tinytl/common.h"
#ifdef PLATFORM_WINDOWS 
struct EventIn
{
    INPUT_RECORD eventsRecord[MAX_EVENT_RECEIVED];
    HANDLE hInput; 
    DWORD numEvents;  // How many events have happened?
    DWORD numEventsRead; // How many events have we read from the console?
};

inline void IO_Console_PollEvents(EventIn& event)
{
    GetNumberOfConsoleInputEvents(event.hInput, &event.numEvents);
    if(event.numEvents != 0)
    {
        ReadConsoleInput(event.hInput, event.eventsRecord, event.numEvents, &event.numEventsRead);
    }
}
#endif
#endif