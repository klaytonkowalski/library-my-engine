////////////////////////////////////////////////////////////////////////////////
// License
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

#ifndef MY_GUARD_MYENGINE
#define MY_GUARD_MYENGINE

////////////////////////////////////////////////////////////////////////////////
// Dependencies
////////////////////////////////////////////////////////////////////////////////

#include <myengine/export.h>

#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////

#define MY_DEFAULT_TEXTURE 1
#define MY_DEFAULT_SHADER_ENTITY 1

#define MY_COLOR_WHITE (MyColor) { 1.0f, 1.0f, 1.0f, 1.0f }
#define MY_COLOR_BLACK (MyColor) { 0.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_RED (MyColor) { 1.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_GREEN (MyColor) { 0.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_BLUE (MyColor) { 0.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_YELLOW (MyColor) { 1.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_MAGENTA (MyColor) { 1.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_CYAN (MyColor) { 0.0f, 1.0f, 1.0f, 1.0f }

#define MY_FLOAT_RADIANS 0.01745329251f
#define MY_FLOAT_DEGREES 57.2957795131f
#define MY_FLOAT_PI 3.14159265359f

#define MY_VECTOR_ZERO (MyVector) { 0.0f, 0.0f, 0.0f }
#define MY_VECTOR_BASIS_X (MyVector) { 1.0f, 0.0f, 0.0f }
#define MY_VECTOR_BASIS_Y (MyVector) { 0.0f, 1.0f, 0.0f }
#define MY_VECTOR_BASIS_Z (MyVector) { 0.0f, 0.0f, -1.0f }

#define MY_TRANSFORM_IDENTITY (MyTransform) { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

typedef int MyHandle;

typedef void (*MyClockCallback)(MyHandle clockHandle);

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

typedef struct MyVector
{
    float x;
    float y;
    float z;
}
MyVector;

typedef struct MyTransform
{
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
    float m4, m8, m12, m16;
}
MyTransform;

////////////////////////////////////////////////////////////////////////////////
// Window Functions
////////////////////////////////////////////////////////////////////////////////

MY_API bool my_window_create(int x, int y, int width, int height, const char* title);
MY_API void my_window_destroy(void);
MY_API bool my_window_prepare(void);
MY_API void my_window_render(void);
MY_API void my_window_position(int x, int y);
MY_API void my_window_size(int width, int height);
MY_API void my_window_title(const char* title);
MY_API void my_window_color(MyColor color);
MY_API void my_window_viewport(float x, float y, float width, float height);
MY_API void my_window_vsync(bool vsync);
MY_API void my_window_depth(bool depth);
MY_API void my_window_cursor(bool cursor);

MY_API float my_window_get_cursor_x(void);
MY_API float my_window_get_cursor_y(void);
MY_API float my_window_get_cursor_delta_x(void);
MY_API float my_window_get_cursor_delta_y(void);
MY_API MyKeyState my_window_get_key_state(MyKey key);
MY_API int my_window_get_frame_rate(void);

////////////////////////////////////////////////////////////////////////////////
// Texture Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyHandle my_texture_create(const char* imagePath, int frameCount);
MY_API void my_texture_destroy(MyHandle textureHandle);
MY_API void my_texture_frame(MyHandle textureHandle, int frameIndex, int x, int y, int width, int height);

////////////////////////////////////////////////////////////////////////////////
// Shader Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyHandle my_shader_create(const char* vertexPath, const char* fragmentPath);
MY_API void my_shader_destroy(MyHandle shaderHandle);

////////////////////////////////////////////////////////////////////////////////
// Clock Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyHandle my_clock_create(void);
MY_API void my_clock_destroy(MyHandle clockHandle);
MY_API void my_clock_start(MyHandle clockHandle);
MY_API void my_clock_stop(MyHandle clockHandle);
MY_API void my_clock_reset(MyHandle clockHandle);
MY_API void my_clock_interval(MyHandle clockHandle, float interval);
MY_API void my_clock_callback(MyHandle clockHandle, MyClockCallback callback);

MY_API float my_clock_get_time(MyHandle clockHandle);
MY_API float my_clock_get_progress(MyHandle clockHandle);

////////////////////////////////////////////////////////////////////////////////
// Float Functions
////////////////////////////////////////////////////////////////////////////////

MY_API float my_float_randomize(float floor, float ceiling);
MY_API float my_float_clamp(float value, float floor, float ceiling);
MY_API float my_float_wrap(float value, float floor, float ceiling);

////////////////////////////////////////////////////////////////////////////////
// Color Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyColor my_color_uniform(float value, bool alpha);
MY_API MyColor my_color_randomize(bool alpha);
MY_API MyColor my_color_clamp(MyColor color);

////////////////////////////////////////////////////////////////////////////////
// Vector Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyVector my_vector_uniform(float value);
MY_API MyVector my_vector_randomize(MyVector floor, MyVector ceiling);
MY_API MyVector my_vector_add(MyVector lhs, MyVector rhs);
MY_API MyVector my_vector_subtract(MyVector lhs, MyVector rhs);
MY_API MyVector my_vector_scale_uniform(MyVector vector, float factor);
MY_API MyVector my_vector_scale_nonuniform(MyVector lhs, MyVector rhs);
MY_API MyVector my_vector_negate(MyVector vector);
MY_API float my_vector_length(MyVector vector);
MY_API MyVector my_vector_normalize(MyVector vector);
MY_API float my_vector_dot(MyVector lhs, MyVector rhs);
MY_API MyVector my_vector_cross(MyVector lhs, MyVector rhs);
MY_API MyVector my_vector_rotate(MyVector vector, MyVector rotation);
MY_API MyVector my_vector_clamp(MyVector vector, MyVector floor, MyVector ceiling);
MY_API MyVector my_vector_wrap(MyVector vector, MyVector floor, MyVector ceiling);
MY_API void my_vector_basis(MyVector* basisX, MyVector* basisY, MyVector* basisZ, MyVector rotation);

////////////////////////////////////////////////////////////////////////////////
// Transform Functions
////////////////////////////////////////////////////////////////////////////////

MY_API MyTransform my_transform_multiply(MyTransform lhs, MyTransform rhs);
MY_API MyTransform my_transform_translate(MyTransform transform, MyVector translation);
MY_API MyTransform my_transform_scale_uniform(MyTransform transform, float factor);
MY_API MyTransform my_transform_scale_nonuniform(MyTransform transform, MyVector factor);
MY_API MyTransform my_transform_rotate(MyTransform transform, MyVector rotation);

////////////////////////////////////////////////////////////////////////////////
// File Functions
////////////////////////////////////////////////////////////////////////////////

MY_API char* my_file_read(const char* path);
MY_API char* my_file_extension(const char* path);

#endif