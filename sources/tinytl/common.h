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

enum class CharacterEncoding {
    ASCII,
    UTF8,
    UTF16
};

enum class PixelFormat {
    RGB,
    RGBA,
    Grayscale
};


#endif