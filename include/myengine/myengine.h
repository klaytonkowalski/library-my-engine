// Copyright (c) 2023 Klayton Kowalski
// 
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.

#ifndef MYENGINE_GUARD
#define MYENGINE_GUARD

#include <myengine/export.h>

#include <stdbool.h>

#define MY_COLOR_WHITE (MyColor) { 1.0f, 1.0f, 1.0f, 1.0f }
#define MY_COLOR_BLACK (MyColor) { 0.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_RED (MyColor) { 1.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_GREEN (MyColor) { 0.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_BLUE (MyColor) { 0.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_YELLOW (MyColor) { 1.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_MAGENTA (MyColor) { 1.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_CYAN (MyColor) { 0.0f, 1.0f, 1.0f, 1.0f }

typedef enum MyKey
{
    MY_KEY_SPACE,
    MY_KEY_APOSTROPHE,
    MY_KEY_COMMA,
    MY_KEY_MINUS,
    MY_KEY_PERIOD,
    MY_KEY_SLASH,
    MY_KEY_0,
    MY_KEY_1,
    MY_KEY_2,
    MY_KEY_3,
    MY_KEY_4,
    MY_KEY_5,
    MY_KEY_6,
    MY_KEY_7,
    MY_KEY_8,
    MY_KEY_9,
    MY_KEY_SEMICOLON,
    MY_KEY_EQUAL,
    MY_KEY_A,
    MY_KEY_B,
    MY_KEY_C,
    MY_KEY_D,
    MY_KEY_E,
    MY_KEY_F,
    MY_KEY_G,
    MY_KEY_H,
    MY_KEY_I,
    MY_KEY_J,
    MY_KEY_K,
    MY_KEY_L,
    MY_KEY_M,
    MY_KEY_N,
    MY_KEY_O,
    MY_KEY_P,
    MY_KEY_Q,
    MY_KEY_R,
    MY_KEY_S,
    MY_KEY_T,
    MY_KEY_U,
    MY_KEY_V,
    MY_KEY_W,
    MY_KEY_X,
    MY_KEY_Y,
    MY_KEY_Z,
    MY_KEY_LEFT_BRACKET,
    MY_KEY_BACKSLASH,
    MY_KEY_RIGHT_BRACKET,
    MY_KEY_GRAVE_ACCENT,
    MY_KEY_ESCAPE,
    MY_KEY_ENTER,
    MY_KEY_TAB,
    MY_KEY_BACKSPACE,
    MY_KEY_INSERT,
    MY_KEY_DELETE,
    MY_KEY_RIGHT,
    MY_KEY_LEFT,
    MY_KEY_DOWN,
    MY_KEY_UP,
    MY_KEY_PAGE_UP,
    MY_KEY_PAGE_DOWN,
    MY_KEY_HOME,
    MY_KEY_END,
    MY_KEY_CAPS_LOCK,
    MY_KEY_SCROLL_LOCK,
    MY_KEY_NUM_LOCK,
    MY_KEY_PRINT_SCREEN,
    MY_KEY_PAUSE,
    MY_KEY_F1,
    MY_KEY_F2,
    MY_KEY_F3,
    MY_KEY_F4,
    MY_KEY_F5,
    MY_KEY_F6,
    MY_KEY_F7,
    MY_KEY_F8,
    MY_KEY_F9,
    MY_KEY_F10,
    MY_KEY_F11,
    MY_KEY_F12,
    MY_KEY_KP_0,
    MY_KEY_KP_1,
    MY_KEY_KP_2,
    MY_KEY_KP_3,
    MY_KEY_KP_4,
    MY_KEY_KP_5,
    MY_KEY_KP_6,
    MY_KEY_KP_7,
    MY_KEY_KP_8,
    MY_KEY_KP_9,
    MY_KEY_KP_DECIMAL,
    MY_KEY_KP_DIVIDE,
    MY_KEY_KP_MULTIPLY,
    MY_KEY_KP_SUBTRACT,
    MY_KEY_KP_ADD,
    MY_KEY_KP_ENTER,
    MY_KEY_KP_EQUAL,
    MY_KEY_LEFT_SHIFT,
    MY_KEY_LEFT_CONTROL,
    MY_KEY_LEFT_ALT,
    MY_KEY_LEFT_SUPER,
    MY_KEY_RIGHT_SHIFT,
    MY_KEY_RIGHT_CONTROL,
    MY_KEY_RIGHT_ALT,
    MY_KEY_RIGHT_SUPER,
    MY_KEY_MENU,
    MY_KEY_COUNT
}
MyKey;

typedef enum MyKeyState
{
    MY_KEY_STATE_UP,
    MY_KEY_STATE_PRESSED,
    MY_KEY_STATE_DOWN,
    MY_KEY_STATE_RELEASED
}
MyKeyState;

typedef struct MyColor
{
    float red;
    float green;
    float blue;
    float alpha;
}
MyColor;

MYENGINE_API bool my_window_create (int x, int y, int width, int height, const char* title);
MYENGINE_API void my_window_destroy (void);
MYENGINE_API bool my_window_prepare (void);
MYENGINE_API void my_window_render (void);
MYENGINE_API void my_window_position (int x, int y);
MYENGINE_API void my_window_size (int width, int height);
MYENGINE_API void my_window_title (const char* title);
MYENGINE_API void my_window_color (MyColor color);
MYENGINE_API void my_window_viewport (float x, float y, float width, float height);
MYENGINE_API void my_window_vsync (bool vsync);
MYENGINE_API void my_window_depth (bool depth);
MYENGINE_API void my_window_cursor (bool cursor);

MYENGINE_API float my_window_get_cursor_x (void);
MYENGINE_API float my_window_get_cursor_y (void);
MYENGINE_API float my_window_get_cursor_delta_x (void);
MYENGINE_API float my_window_get_cursor_delta_y (void);
MYENGINE_API MyKeyState my_window_get_key_state (MyKey key);

MYENGINE_API MyColor my_color_uniform (float value, bool alpha);
MYENGINE_API MyColor my_color_randomize (bool alpha);
MYENGINE_API MyColor my_color_clamp (MyColor color);

#endif