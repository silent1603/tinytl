#ifndef RENDERER_COMMON_H
#define RENDERER_COMMON_H
#include "renderer_platform_selector.h"
#include "renderer_hardware_backend.h"
#include "tinytl/container/vector.h"

// ############################# CONSOLE VARIABLES #######################################

#define SUPPORT_CONSOLE_TITLE_NAME_SIZE 128
#define MIN_FONT_WIDTH  2 
#define MIN_FONT_HEIGHT  5 
#define SUPPORT_SCREEN_WIDTH_PIXELS   3840 
#define SUPPORT_SCREEN_HEIGHT_PIXELS  2160 

#define MAX_CONSOLE_COLS  (SUPPORT_SCREEN_WIDTH_PIXELS / MIN_FONT_WIDTH)    // 960
#define MAX_CONSOLE_ROWS  (SUPPORT_SCREEN_HEIGHT_PIXELS / MIN_FONT_HEIGHT)  // 360

// ########################################################################################



#endif