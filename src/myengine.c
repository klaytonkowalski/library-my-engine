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
// Dependencies
////////////////////////////////////////////////////////////////////////////////

#include <myengine/myengine.h>

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////

#define MY_INVALID_HANDLE 0

#define MY_DEFAULT_CLOCK 1

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

typedef struct MyClock
{
    MyHandle clockHandle;
    MyClockCallback callback;
    float lastTime;
    float totalTime;
    float interval;
    float intervalTime;
    bool active;
}
MyClock;

typedef struct MyEngine
{
    GLFWwindow* window;
    MyClock clocks[MY_OPTION_CAPACITY_CLOCK];
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    float viewportX;
    float viewportY;
    float viewportWidth;
    float viewportHeight;
    double cursorX;
    double cursorY;
    double cursorDeltaX;
    double cursorDeltaY;
    MyKey keyStates[MY_KEY_COUNT];
    GLbitfield renderMask;
    int frameCount;
    int frameRate;
}
MyEngine;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void my_window_position_callback(GLFWwindow* window, int x, int y);
static void my_window_size_callback(GLFWwindow* window, int width, int height);

static void my_clock_frame_callback(MyHandle clockHandle);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static MyEngine myEngine = { 0 };

static const int myKeys[] =
{
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_SEMICOLON,
    GLFW_KEY_EQUAL,
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_BACKSLASH,
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_INSERT,
    GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,
    GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN,
    GLFW_KEY_HOME,
    GLFW_KEY_END,
    GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK,
    GLFW_KEY_NUM_LOCK,
    GLFW_KEY_PRINT_SCREEN,
    GLFW_KEY_PAUSE,
    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    GLFW_KEY_KP_0,
    GLFW_KEY_KP_1,
    GLFW_KEY_KP_2,
    GLFW_KEY_KP_3,
    GLFW_KEY_KP_4,
    GLFW_KEY_KP_5,
    GLFW_KEY_KP_6,
    GLFW_KEY_KP_7,
    GLFW_KEY_KP_8,
    GLFW_KEY_KP_9,
    GLFW_KEY_KP_DECIMAL,
    GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY,
    GLFW_KEY_KP_SUBTRACT,
    GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER,
    GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT,
    GLFW_KEY_RIGHT_SUPER,
    GLFW_KEY_MENU
};

////////////////////////////////////////////////////////////////////////////////
// Window Functions
////////////////////////////////////////////////////////////////////////////////

bool my_window_create(int x, int y, int width, int height, const char* title)
{
    if (!glfwInit())
    {
        my_window_destroy();
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    myEngine.window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!myEngine.window)
    {
        my_window_destroy();
        return false;
    }
    glfwSetWindowPosCallback(myEngine.window, my_window_position_callback);
    glfwSetWindowSizeCallback(myEngine.window, my_window_size_callback);
    my_window_position(x, y);
    glfwMakeContextCurrent(myEngine.window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        my_window_destroy();
        return false;
    }
    my_window_color(MY_COLOR_BLACK);
    my_window_viewport(0.0f, 0.0f, 1.0f, 1.0f);
    my_window_vsync(true);
    my_window_depth(true);
    myEngine.renderMask = GL_COLOR_BUFFER_BIT;
    if (!my_clock_create())
    {
        my_window_destroy();
        return false;
    }
    my_clock_interval(MY_DEFAULT_CLOCK, 1.0f);
    my_clock_callback(MY_DEFAULT_CLOCK, my_clock_frame_callback);
    my_clock_start(MY_DEFAULT_CLOCK);
    stbi_set_flip_vertically_on_load(true);
    return true;
}

void my_window_destroy(void)
{
    if (myEngine.window)
    {
        glfwDestroyWindow(myEngine.window);
    }
    glfwTerminate();
    myEngine = (MyEngine) { 0 };
}

bool my_window_prepare(void)
{
    glfwPollEvents();
    if (glfwWindowShouldClose(myEngine.window))
    {
        return false;
    }
    double cursorX, cursorY;
    glfwGetCursorPos(myEngine.window, &cursorX, &cursorY);
    myEngine.cursorDeltaX = cursorX - myEngine.cursorX;
    myEngine.cursorDeltaY = myEngine.windowHeight - cursorY - myEngine.cursorY;
    myEngine.cursorX = cursorX;
    myEngine.cursorY = myEngine.windowHeight - cursorY;
    for (int i = 0; i < MY_KEY_COUNT; i++)
    {
        const int state = glfwGetKey(myEngine.window, myKeys[i]);
        if (state == GLFW_PRESS)
        {
            if (myEngine.keyStates[i] == MY_KEY_STATE_UP || myEngine.keyStates[i] == MY_KEY_STATE_RELEASED)
            {
                myEngine.keyStates[i] = MY_KEY_STATE_PRESSED;
            }
            else if (myEngine.keyStates[i] == MY_KEY_STATE_PRESSED)
            {
                myEngine.keyStates[i] = MY_KEY_STATE_DOWN;
            }
        }
        else if (state == GLFW_RELEASE)
        {
            if (myEngine.keyStates[i] == MY_KEY_STATE_DOWN || myEngine.keyStates[i] == MY_KEY_STATE_PRESSED)
            {
                myEngine.keyStates[i] = MY_KEY_STATE_RELEASED;
            }
            else if (myEngine.keyStates[i] == MY_KEY_STATE_RELEASED)
            {
                myEngine.keyStates[i] = MY_KEY_STATE_UP;
            }
        }
    }
    glfwSwapBuffers(myEngine.window);
    glClear(myEngine.renderMask);
    return true;
}

void my_window_render(void)
{

}

void my_window_position(int x, int y)
{
    glfwSetWindowPos(myEngine.window, myEngine.windowX, myEngine.windowY);
}

void my_window_size(int width, int height)
{
    glfwSetWindowSize(myEngine.window, width, height);
}

void my_window_title(const char* title)
{
    glfwSetWindowTitle(myEngine.window, title);
}

void my_window_color(MyColor color)
{
    color = my_color_clamp(color);
    glClearColor(color.red, color.green, color.blue, color.alpha);
}

void my_window_viewport(float x, float y, float width, float height)
{
    myEngine.viewportX = x;
    myEngine.viewportY = y;
    myEngine.viewportWidth = width;
    myEngine.viewportHeight = height;
    glViewport(myEngine.windowWidth * x, myEngine.windowHeight * y, myEngine.windowWidth * width, myEngine.windowHeight * height);
}

void my_window_vsync(bool vsync)
{
    glfwSwapInterval(vsync ? 1 : 0);
}

void my_window_depth(bool depth)
{
    if (depth)
    {
        myEngine.renderMask |= GL_DEPTH_BUFFER_BIT;
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        myEngine.renderMask &= ~GL_DEPTH_BUFFER_BIT;
        glDisable(GL_DEPTH_TEST);
    }
}

void my_window_cursor(bool cursor)
{
    glfwSetInputMode(myEngine.window, GLFW_CURSOR, cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

float my_window_get_cursor_x(void)
{
    return myEngine.cursorX;
}

float my_window_get_cursor_y(void)
{
    return myEngine.cursorY;
}

float my_window_get_cursor_delta_x(void)
{
    return myEngine.cursorDeltaX;
}

float my_window_get_cursor_delta_y(void)
{
    return myEngine.cursorDeltaY;
}

MyKeyState my_window_get_key_state(MyKey key)
{
    return myEngine.keyStates[key];
}

static void my_window_position_callback(GLFWwindow* window, int x, int y)
{
    myEngine.windowX = x;
    myEngine.windowY = y;
}

static void my_window_size_callback(GLFWwindow* window, int width, int height)
{
    myEngine.windowWidth = width;
    myEngine.windowHeight = height;
    glViewport(myEngine.windowWidth * myEngine.viewportX, myEngine.windowHeight * myEngine.viewportY, myEngine.viewportWidth * width, myEngine.viewportHeight * height);
}

////////////////////////////////////////////////////////////////////////////////
// Clock Functions
////////////////////////////////////////////////////////////////////////////////

MyHandle my_clock_create(void)
{
    MyHandle clockHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < MY_OPTION_CAPACITY_CLOCK; i++)
    {
        if (!myEngine.clocks[i].clockHandle)
        {
            clockHandle = i;
            break;
        }
    }
    if (!clockHandle)
    {
        return MY_INVALID_HANDLE;
    }
    myEngine.clocks[clockHandle].clockHandle = clockHandle;
    return clockHandle;
}

void my_clock_destroy(MyHandle clockHandle)
{
    myEngine.clocks[clockHandle] = (MyClock) { 0 };
}

void my_clock_start(MyHandle clockHandle)
{
    myEngine.clocks[clockHandle].lastTime = (float) glfwGetTime();
    myEngine.clocks[clockHandle].active = true;
}

void my_clock_stop(MyHandle clockHandle)
{
    myEngine.clocks[clockHandle].active = false;
}

void my_clock_reset(MyHandle clockHandle)
{
    myEngine.clocks[clockHandle].lastTime = (float) glfwGetTime();
    myEngine.clocks[clockHandle].totalTime = 0.0f;
    myEngine.clocks[clockHandle].intervalTime = 0.0f;
}

void my_clock_interval(MyHandle clockHandle, float interval)
{
    myEngine.clocks[clockHandle].interval = interval;
    myEngine.clocks[clockHandle].intervalTime = 0.0f;
}

void my_clock_callback(MyHandle clockHandle, MyClockCallback callback)
{
    myEngine.clocks[clockHandle].callback = callback;
}

float my_clock_get_time(MyHandle clockHandle)
{
    return myEngine.clocks[clockHandle].totalTime;
}

float my_clock_get_progress(MyHandle clockHandle)
{
    return myEngine.clocks[clockHandle].intervalTime / myEngine.clocks[clockHandle].interval;
}

static void my_clock_frame_callback(MyHandle clockHandle)
{
    myEngine.frameRate = myEngine.frameCount;
    myEngine.frameCount = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Float Functions
////////////////////////////////////////////////////////////////////////////////

float my_float_randomize(float floor, float ceiling)
{
    const float range = ceiling - floor;
    return (float) rand() / RAND_MAX * range + floor;
}

float my_float_clamp(float value, float floor, float ceiling)
{
    if (value < floor)
    {
        return floor;
    }
    if (value > ceiling)
    {
        return ceiling;
    }
    return value;
}

float my_float_wrap(float value, float floor, float ceiling)
{
    const float range = ceiling - floor;
    while (value < floor)
    {
        value += range;
    }
    while (value > ceiling)
    {
        value -= range;
    }
    return value;
}

////////////////////////////////////////////////////////////////////////////////
// Color Functions
////////////////////////////////////////////////////////////////////////////////

MyColor my_color_uniform(float value, bool alpha)
{
    return (MyColor)
    {
        value,
        value,
        value,
        alpha ? value : 1.0f
    };
}

MyColor my_color_randomize(bool alpha)
{
    return (MyColor)
    {
        (float) rand() / RAND_MAX,
        (float) rand() / RAND_MAX,
        (float) rand() / RAND_MAX,
        alpha ? (float) rand() / RAND_MAX : 1.0f
    };
}

MyColor my_color_clamp(MyColor color)
{
    return (MyColor)
    {
        my_float_clamp(color.red, 0.0f, 1.0f),
        my_float_clamp(color.green, 0.0f, 1.0f),
        my_float_clamp(color.blue, 0.0f, 1.0f),
        my_float_clamp(color.alpha, 0.0f, 1.0f)
    };
}

////////////////////////////////////////////////////////////////////////////////
// Vector Functions
////////////////////////////////////////////////////////////////////////////////

MyVector my_vector_uniform(float value)
{
    return (MyVector)
    {
        value,
        value,
        value
    };
}

MyVector my_vector_randomize(MyVector floor, MyVector ceiling)
{
    return (MyVector)
    {
        my_float_randomize(floor.x, ceiling.x),
        my_float_randomize(floor.y, ceiling.y),
        my_float_randomize(floor.z, ceiling.z)
    };
}

MyVector my_vector_add(MyVector lhs, MyVector rhs)
{
    return (MyVector)
    {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z
    };
}

MyVector my_vector_subtract(MyVector lhs, MyVector rhs)
{
    return (MyVector)
    {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z
    };
}

MyVector my_vector_scale_uniform(MyVector vector, float factor)
{
    return (MyVector)
    {
        vector.x * factor,
        vector.y * factor,
        vector.z * factor
    };
}

MyVector my_vector_scale_nonuniform(MyVector lhs, MyVector rhs)
{
    return (MyVector)
    {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    };
}

MyVector my_vector_negate(MyVector vector)
{
    return (MyVector)
    {
        -vector.x,
        -vector.y,
        -vector.z,
    };
}

float my_vector_length(MyVector vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

MyVector my_vector_normalize(MyVector vector)
{
    const float length = my_vector_length(vector);
    if (length == 0.0f)
    {
        return MY_VECTOR_ZERO;
    }
    return my_vector_scale_uniform(vector, 1.0f / length);
}

float my_vector_dot(MyVector lhs, MyVector rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

MyVector my_vector_cross(MyVector lhs, MyVector rhs)
{
    return (MyVector)
    {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

MyVector my_vector_rotate(MyVector vector, MyVector rotation)
{
    if (rotation.x)
    {
        const float radians = rotation.x * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(radians);
        const float sinRadians = sinf(radians);
        vector = (MyVector)
        {
            vector.x,
            vector.y * cosRadians + vector.z * -sinRadians,
            vector.y * sinRadians + vector.z * cosRadians
        };
    }
    if (rotation.y)
    {
        const float radians = rotation.y * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(radians);
        const float sinRadians = sinf(radians);
        vector = (MyVector)
        {
            vector.x * cosRadians + vector.z * sinRadians,
            vector.y,
            vector.x * -sinRadians + vector.z * cosRadians
        };
    }
    if (rotation.z)
    {
        const float radians = rotation.z * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(radians);
        const float sinRadians = sinf(radians);
        vector = (MyVector)
        {
            vector.x * cosRadians + vector.y * -sinRadians,
            vector.x * sinRadians + vector.y * cosRadians,
            vector.z
        };
    }
    return vector;
}

MyVector my_vector_clamp(MyVector vector, MyVector floor, MyVector ceiling)
{
    return (MyVector)
    {
        my_float_clamp(vector.x, floor.x, ceiling.x),
        my_float_clamp(vector.y, floor.y, ceiling.y),
        my_float_clamp(vector.z, floor.z, ceiling.z)
    };
}

MyVector my_vector_wrap(MyVector vector, MyVector floor, MyVector ceiling)
{
    return (MyVector)
    {
        my_float_wrap(vector.x, floor.x, ceiling.x),
        my_float_wrap(vector.y, floor.y, ceiling.y),
        my_float_wrap(vector.z, floor.z, ceiling.z)
    };
}

void my_vector_basis(MyVector* basisX, MyVector* basisY, MyVector* basisZ, MyVector rotation)
{
    *basisZ = my_vector_normalize(my_vector_rotate(MY_VECTOR_BASIS_Z, rotation));
    *basisX = my_vector_normalize(my_vector_cross(*basisZ, MY_VECTOR_BASIS_Y));
    *basisY = my_vector_normalize(my_vector_cross(*basisX, *basisZ));
}

////////////////////////////////////////////////////////////////////////////////
// Transform Functions
////////////////////////////////////////////////////////////////////////////////

MyTransform my_transform_multiply(MyTransform lhs, MyTransform rhs)
{
    return (MyTransform)
    {
        lhs.m1 * rhs.m1 + lhs.m2 * rhs.m5 + lhs.m3 * rhs.m9  + lhs.m4 * rhs.m13,
        lhs.m5 * rhs.m1 + lhs.m6 * rhs.m5 + lhs.m7 * rhs.m9  + lhs.m8 * rhs.m13,
        lhs.m9 * rhs.m1 + lhs.m10 * rhs.m5 + lhs.m11 * rhs.m9  + lhs.m12 * rhs.m13,
        lhs.m13 * rhs.m1 + lhs.m14 * rhs.m5 + lhs.m15 * rhs.m9  + lhs.m16 * rhs.m13,
        lhs.m1 * rhs.m2 + lhs.m2 * rhs.m6 + lhs.m3 * rhs.m10 + lhs.m4 * rhs.m14,
        lhs.m5 * rhs.m2 + lhs.m6 * rhs.m6 + lhs.m7 * rhs.m10 + lhs.m8 * rhs.m14,
        lhs.m9 * rhs.m2 + lhs.m10 * rhs.m6 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m14,
        lhs.m13 * rhs.m2 + lhs.m14 * rhs.m6 + lhs.m15 * rhs.m10 + lhs.m16 * rhs.m14,
        lhs.m1 * rhs.m3 + lhs.m2 * rhs.m7 + lhs.m3 * rhs.m11 + lhs.m4 * rhs.m15,
        lhs.m5 * rhs.m3 + lhs.m6 * rhs.m7 + lhs.m7 * rhs.m11 + lhs.m8 * rhs.m15,
        lhs.m9 * rhs.m3 + lhs.m10 * rhs.m7 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m15,
        lhs.m13 * rhs.m3 + lhs.m14 * rhs.m7 + lhs.m15 * rhs.m11 + lhs.m16 * rhs.m15,
        lhs.m1 * rhs.m4 + lhs.m2 * rhs.m8 + lhs.m3 * rhs.m12 + lhs.m4 * rhs.m16,
        lhs.m5 * rhs.m4 + lhs.m6 * rhs.m8 + lhs.m7 * rhs.m12 + lhs.m8 * rhs.m16,
        lhs.m9 * rhs.m4 + lhs.m10 * rhs.m8 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m16,
        lhs.m13 * rhs.m4 + lhs.m14 * rhs.m8 + lhs.m15 * rhs.m12 + lhs.m16 * rhs.m16
    };
}

MyTransform my_transform_translate(MyTransform transform, MyVector translation)
{
    transform.m1 += translation.x;
    transform.m6 += translation.y;
    transform.m11 += translation.z;
    return transform;
}

MyTransform my_transform_scale_uniform(MyTransform transform, float factor)
{
    transform.m1 *= factor;
    transform.m6 *= factor;
    transform.m11 *= factor;
    return transform;
}

MyTransform my_transform_scale_nonuniform(MyTransform transform, MyVector factor)
{
    transform.m1 *= factor.x;
    transform.m6 *= factor.y;
    transform.m11 *= factor.z;
    return transform;
}

MyTransform my_transform_rotate(MyTransform transform, MyVector rotation)
{
    if (rotation.x)
    {
        const float angleRadians = rotation.x * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(angleRadians);
        const float sinRadians = sinf(angleRadians);
        const MyTransform pitchTransform =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosRadians, -sinRadians, 0.0f,
            0.0f, sinRadians, cosRadians, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        transform = my_transform_multiply(transform, pitchTransform);
    }
    if (rotation.y)
    {
        const float angleRadians = rotation.y * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(angleRadians);
        const float sinRadians = sinf(angleRadians);
        const MyTransform pitchTransform =
        {
            cosRadians, 0.0f, -sinRadians, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            sinRadians, 0.0f, 0.0f, cosRadians,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        transform = my_transform_multiply(transform, pitchTransform);
    }
    if (rotation.z)
    {
        const float angleRadians = rotation.z * MY_FLOAT_RADIANS;
        const float cosRadians = cosf(angleRadians);
        const float sinRadians = sinf(angleRadians);
        const MyTransform pitchTransform =
        {
            cosRadians, -sinRadians, 0.0f, 0.0f,
            sinRadians, cosRadians, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        transform = my_transform_multiply(transform, pitchTransform);
    }
    return transform;
}

////////////////////////////////////////////////////////////////////////////////
// File Functions
////////////////////////////////////////////////////////////////////////////////

char* my_file_read(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    const long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* result = malloc((length + 1) * sizeof(char));
    if (!result)
    {
        return NULL;
    }
    fread(result, length, 1, file);
    fclose(file);
    result[length] = '\0';
    return result;
}

char* my_file_extension(const char* path)
{
    return strrchr(path, '.');
}