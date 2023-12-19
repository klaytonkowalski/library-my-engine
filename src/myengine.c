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

#include <myengine/myengine.h>

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct MyEngine
{
    GLFWwindow* window;
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
}
MyEngine;

static void my_window_position_callback(GLFWwindow* window, int x, int y);
static void my_window_size_callback(GLFWwindow* window, int width, int height);

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

MyColor my_color_uniform(float value, bool alpha)
{
    return (MyColor) { value, value, value, alpha ? value : 1.0f };
}

MyColor my_color_randomize(bool alpha)
{
    return (MyColor) { (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX, alpha ? (float) rand() / RAND_MAX : 1.0f };
}

MyColor my_color_clamp(MyColor color)
{
    if (color.red < 0.0f)
    {
        color.red = 0.0f;
    }
    else if (color.red > 1.0f)
    {
        color.red = 1.0f;
    }
    if (color.green < 0.0f)
    {
        color.green = 0.0f;
    }
    else if (color.green > 1.0f)
    {
        color.green = 1.0f;
    }
    if (color.blue < 0.0f)
    {
        color.blue = 0.0f;
    }
    else if (color.blue > 1.0f)
    {
        color.blue = 1.0f;
    }
    return color;
}