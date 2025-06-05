#ifndef COMMON_H
#define COMMON_H
#include <float.h>
#include <limits.h>
#include <iso646.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
#include <stdnoreturn.h>

enum CharacterEncoding {
    ASCII,
    UTF8,
    UTF16
};

enum PixelFormat {
    RGB,
    RGBA,
    Grayscale
};

// ############################# CONSOLE VARIABLES #######################################

#define SUPPORT_CONSOLE_TITLE_NAME_SIZE 128
#define MIN_FONT_WIDTH  2 
#define MIN_FONT_HEIGHT  5 
#define SUPPORT_SCREEN_WIDTH_PIXELS   3840 
#define SUPPORT_SCREEN_HEIGHT_PIXELS  2160 

#define MAX_CONSOLE_COLS  (SUPPORT_SCREEN_WIDTH_PIXELS / MIN_FONT_WIDTH)    // 960
#define MAX_CONSOLE_ROWS  (SUPPORT_SCREEN_HEIGHT_PIXELS / MIN_FONT_HEIGHT)  // 360

// ########################################################################################

#define MAX_EVENT_RECEIVED 128

#endif