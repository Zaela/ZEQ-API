
#ifndef _ZEQ_WINDOW_H_
#define _ZEQ_WINDOW_H_

#include "zeq.h"

typedef struct zeq_window_t zeq_window_t;

enum zeq_window_options
{
    ZEQ_WINDOW_FULLSCREEN               = 1 << 0,
    ZEQ_WINDOW_HIDE_TITLEBAR            = 1 << 1,
    ZEQ_WINDOW_USE_NATIVE_ASPECT_RATIO  = 1 << 2,
    ZEQ_WINDOW_VSYNC                    = 1 << 3
};

ZEQ_API zeq_window_t*       zeq_window_create(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options);
ZEQ_API void                zeq_window_destroy(zeq_window_t* window);

ZEQ_API void                zeq_window_clear(void);
ZEQ_API void                zeq_window_display(zeq_window_t* window);

#endif//_ZEQ_WINDOW_H_
