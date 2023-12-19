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

////////////////////////////////////////////////////////////////////////////////
// We define a custom boolean type because we don't want to force the user to include the "stdbool.h" header in their project.
// We also do it because that's what most C libraries do and we shouldn't break the norm in that respect.

typedef int MyBool;

#define MY_TRUE  (MyBool) 1
#define MY_FALSE (MyBool) 0

////////////////////////////////////////////////////////////////////////////////
// Color data has four channels: red, green, blue, and alpha.
// Every channel is clamped between [0.0f, 1.0f].
// We also provide a list of pre-defined colors, partly for convenience but mostly for fun.

typedef struct MyColor
{
    float red;
    float green;
    float blue;
    float alpha;
}
MyColor;

#define MY_COLOR_WHITE   (MyColor) { 1.0f, 1.0f, 1.0f, 1.0f }
#define MY_COLOR_BLACK   (MyColor) { 0.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_RED     (MyColor) { 1.0f, 0.0f, 0.0f, 1.0f }
#define MY_COLOR_GREEN   (MyColor) { 0.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_BLUE    (MyColor) { 0.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_YELLOW  (MyColor) { 1.0f, 1.0f, 0.0f, 1.0f }
#define MY_COLOR_MAGENTA (MyColor) { 1.0f, 0.0f, 1.0f, 1.0f }
#define MY_COLOR_CYAN    (MyColor) { 0.0f, 1.0f, 1.0f, 1.0f }

////////////////////////////////////////////////////////////////////////////////
// 

MYENGINE_API MyBool my_window_create   (int x, int y, int width, int height, const char* title);
MYENGINE_API void   my_window_destroy  (void);
MYENGINE_API MyBool my_window_prepare  (void);
MYENGINE_API void   my_window_render   (void);
MYENGINE_API void   my_window_position (int x, int y);
MYENGINE_API void   my_window_size     (int width, int height);
MYENGINE_API void   my_window_title    (const char* title);
MYENGINE_API void   my_window_color    (MyColor color);
MYENGINE_API void   my_window_viewport (float x, float y, float width, float height);
MYENGINE_API void   my_window_vsync    (MyBool flag);
MYENGINE_API void   my_window_depth    (MyBool flag);

#endif