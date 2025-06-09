#ifndef WINDOW_DATA_H
#define WINDOW_DATA_H
#include "window_data_win32.h"
#include "window_data_x11.h"
#include "window_data_wayland.h"

enum WindowPropertiesBitIndex
{
    FULLSCREEN = 0,
    VSYNC = 0 << 1
};

enum MousePropertiesBitIndex
{
    LDOWN = 0,
    RDOWN = 0 << 1
};

typedef struct 
{
    int oldMousePos[2];
    int currentMousePos[2];
    int windowSize[2];
    int colorBits[4];
    int depthBits;
    int stencilsBits;
    int numSamples;
    int mouseState;
    int properties;
    WindowNativeData nativeData;
} WindowData;

#endif