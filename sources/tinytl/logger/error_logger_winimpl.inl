#ifndef ERRORLOGGERWINIMPL_H
#define ERRORLOGGERWINIMPL_H
#include <tinytl/platform_selector.h>
#if PLATFORMS_WINDOWS
inline void UIErrorPopup(char *msg)
{
    MessageBoxA(NULL, msg, "Error", MB_ICONERROR);
}
#endif
#endif