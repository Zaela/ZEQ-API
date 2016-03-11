
#ifndef _ZEQ_WINDOW_H_
#define _ZEQ_WINDOW_H_

#include "zeq.h"

typedef struct zeq_window_t zeq_window_t;
typedef struct zeq_input_t  zeq_input_t;

enum zeq_window_options
{
    ZEQ_WINDOW_FULLSCREEN               = 1 << 0,
    ZEQ_WINDOW_HIDE_TITLEBAR            = 1 << 1,
    ZEQ_WINDOW_USE_NATIVE_ASPECT_RATIO  = 1 << 2,
    ZEQ_WINDOW_VSYNC                    = 1 << 3,
    ZEQ_WINDOW_RESIZABLE                = 1 << 4
};

ZEQ_API zeq_window_t*       zeq_window_create(zeq_t* Z, const char* title, uint32_t width, uint32_t height, int options);
ZEQ_API void                zeq_window_destroy(zeq_window_t* window);
ZEQ_API void                zeq_window_resize(zeq_window_t* window, uint32_t width, uint32_t height);
ZEQ_API void                zeq_window_change_viewport(uint32_t width, uint32_t height);

ZEQ_API void                zeq_window_clear(void);
ZEQ_API void                zeq_window_display(zeq_window_t* window);

ZEQ_API int                 zeq_window_poll_input(zeq_window_t* window, zeq_input_t* input);

/* Input types */
typedef struct zeq_mouse_move_t     zeq_mouse_move_t;
typedef struct zeq_mouse_click_t    zeq_mouse_click_t;
typedef struct zeq_mouse_wheel_t    zeq_mouse_wheel_t;
typedef struct zeq_key_press_t      zeq_key_press_t;
typedef struct zeq_text_entry_t     zeq_text_entry_t;
typedef struct zeq_window_resize_t  zeq_window_resize_t;

enum zeq_mouse_button_t
{
    ZEQ_MOUSE_BUTTON_LEFT,
    ZEQ_MOUSE_BUTTON_RIGHT,
    ZEQ_MOUSE_BUTTON_MIDDLE,
    ZEQ_MOUSE_BUTTON_X1,
    ZEQ_MOUSE_BUTTON_X2,
    ZEQ_KEY_ZEQ_MOUSE_BUTTON_UNKNOWN
};

enum zeq_key_t
{
    ZEQ_KEY_UNKNOWN = -1,
    ZEQ_KEY_A       = 0,
    ZEQ_KEY_B,
    ZEQ_KEY_C,
    ZEQ_KEY_D,
    ZEQ_KEY_E,
    ZEQ_KEY_F,
    ZEQ_KEY_G,
    ZEQ_KEY_H,
    ZEQ_KEY_I,
    ZEQ_KEY_J,
    ZEQ_KEY_K,
    ZEQ_KEY_L,
    ZEQ_KEY_M,
    ZEQ_KEY_N,
    ZEQ_KEY_O,
    ZEQ_KEY_P,
    ZEQ_KEY_Q,
    ZEQ_KEY_R,
    ZEQ_KEY_S,
    ZEQ_KEY_T,
    ZEQ_KEY_U,
    ZEQ_KEY_V,
    ZEQ_KEY_W,
    ZEQ_KEY_X,
    ZEQ_KEY_Y,
    ZEQ_KEY_Z,
    ZEQ_KEY_NUM0,
    ZEQ_KEY_NUM1,
    ZEQ_KEY_NUM2,
    ZEQ_KEY_NUM3,
    ZEQ_KEY_NUM4,
    ZEQ_KEY_NUM5,
    ZEQ_KEY_NUM6,
    ZEQ_KEY_NUM7,
    ZEQ_KEY_NUM8,
    ZEQ_KEY_NUM9,
    ZEQ_KEY_ESCAPE,
    ZEQ_KEY_LCTRL,
    ZEQ_KEY_LSHIFT,
    ZEQ_KEY_LALT,
    ZEQ_KEY_LSYS,
    ZEQ_KEY_RCTRL,
    ZEQ_KEY_RSHIFT,
    ZEQ_KEY_RALT,
    ZEQ_KEY_RSYS,
    ZEQ_KEY_MENU,
    ZEQ_KEY_LBRACKET,
    ZEQ_KEY_RBRACKET,
    ZEQ_KEY_SEMICOLON,
    ZEQ_KEY_COMMA,
    ZEQ_KEY_PERIOD,
    ZEQ_KEY_QUOTE,
    ZEQ_KEY_SLASH,
    ZEQ_KEY_BACKSLASH,
    ZEQ_KEY_TILDE,
    ZEQ_KEY_EQUAL,
    ZEQ_KEY_DASH,
    ZEQ_KEY_SPACE,
    ZEQ_KEY_RETURN,
    ZEQ_KEY_BACKSPACE,
    ZEQ_KEY_TAB,
    ZEQ_KEY_PAGEUP,
    ZEQ_KEY_PAGEDOWN,
    ZEQ_KEY_END,
    ZEQ_KEY_HOME,
    ZEQ_KEY_INSERT,
    ZEQ_KEY_DELETE,
    ZEQ_KEY_ADD,
    ZEQ_KEY_SUBTRACT,
    ZEQ_KEY_MULTIPLY,
    ZEQ_KEY_DIVIDE,
    ZEQ_KEY_LEFT,
    ZEQ_KEY_RIGHT,
    ZEQ_KEY_UP,
    ZEQ_KEY_DOWN,
    ZEQ_KEY_NUMPAD0,
    ZEQ_KEY_NUMPAD1,
    ZEQ_KEY_NUMPAD2,
    ZEQ_KEY_NUMPAD3,
    ZEQ_KEY_NUMPAD4,
    ZEQ_KEY_NUMPAD5,
    ZEQ_KEY_NUMPAD6,
    ZEQ_KEY_NUMPAD7,
    ZEQ_KEY_NUMPAD8,
    ZEQ_KEY_NUMPAD9,
    ZEQ_KEY_F1,
    ZEQ_KEY_F2,
    ZEQ_KEY_F3,
    ZEQ_KEY_F4,
    ZEQ_KEY_F5,
    ZEQ_KEY_F6,
    ZEQ_KEY_F7,
    ZEQ_KEY_F8,
    ZEQ_KEY_F9,
    ZEQ_KEY_F10,
    ZEQ_KEY_F11,
    ZEQ_KEY_F12,
    ZEQ_KEY_F13,
    ZEQ_KEY_F14,
    ZEQ_KEY_F15,
    ZEQ_KEY_PAUSE
};

struct zeq_mouse_move_t
{
    int x, y;
};

struct zeq_mouse_click_t
{
    zeq_mouse_button_t  button;
    int                 x, y;
};

struct zeq_mouse_wheel_t
{
    int     isVerticalWheel;
    float   delta;
    int     x, y;
};

struct zeq_key_press_t
{
    zeq_key_t   key;
    int8_t      alt;
    int8_t      ctrl;
    int8_t      shift;
    int8_t      sys;
};

struct zeq_text_entry_t
{
    uint32_t unicode32;
};

struct zeq_window_resize_t
{
    uint32_t width;
    uint32_t height;
};

enum zeq_input_event_t
{
    ZEQ_INPUT_WINDOW_CLOSE,
    ZEQ_INPUT_WINDOW_RESIZE,
    ZEQ_INPUT_WINDOW_LOSE_FOCUS,
    ZEQ_INPUT_WINDOW_GAIN_FOCUS,
    ZEQ_INPUT_MOUSE_ENTER_WINDOW,
    ZEQ_INPUT_MOUSE_LEAVE_WINDOW,
    ZEQ_INPUT_MOUSE_MOVE,
    ZEQ_INPUT_MOUSE_CLICK,
    ZEQ_INPUT_MOUSE_RELEASE,
    ZEQ_INPUT_MOUSE_WHEEL,
    ZEQ_INPUT_KEY_PRESS,
    ZEQ_INPUT_KEY_RELEASE,
    ZEQ_INPUT_TEXT_ENTRY,
    ZEQ_INPUT_UNKNOWN
};

struct zeq_input_t
{
    zeq_input_event_t   event;
    union
    {
        zeq_mouse_move_t    mouseMove;
        zeq_mouse_click_t   mouseClick;
        zeq_mouse_click_t   mouseRelease;
        zeq_mouse_wheel_t   mouseWheel;
        zeq_key_press_t     keyPress;
        zeq_key_press_t     keyRelease;
        zeq_text_entry_t    text;
        zeq_window_resize_t windowResize;
    };
};

/* Provided input handlers */
typedef struct zeq_zoneviewer_t zeq_zoneview_input_handler_t;

ZEQ_API zeq_zoneviewer_t*   zeq_zoneviewer_create(void);
ZEQ_API void                zeq_zoneviewer_destroy(zeq_zoneviewer_t* zoneviewer);
ZEQ_API void                zeq_zoneviewer_handle_input(zeq_zoneviewer_t* zoneviewer, zeq_input_t* input);
ZEQ_API void                zeq_zoneviewer_update(zeq_zoneviewer_t* zoneviewer, zeq_camera_t* camera, zeq_delta_t delta);

typedef struct zeq_orbitviewer_t zeq_orbitviewer_t;

ZEQ_API zeq_orbitviewer_t*  zeq_orbitviewer_create(void);
ZEQ_API void                zeq_orbitviewer_destroy(zeq_orbitviewer_t* orbitviewer);
ZEQ_API void                zeq_orbitviewer_handle_input(zeq_orbitviewer_t* orbitviewer, zeq_input_t* input);
ZEQ_API void                zeq_orbitviewer_update(zeq_orbitviewer_t* orbitviewer, zeq_camera_t* camera, zeq_delta_t delta);

#endif//_ZEQ_WINDOW_H_
