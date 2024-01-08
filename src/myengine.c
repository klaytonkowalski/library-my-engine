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

#define MY_ALLOCATOR_ENTITY 1000
#define MY_ALLOCATOR_TEXTURE 100
#define MY_ALLOCATOR_SHADER 10
#define MY_ALLOCATOR_CLOCK 10
#define MY_ALLOCATOR_BATCH 100
#define MY_ALLOCATOR_BATCH_ENTITY 100
#define MY_ALLOCATOR_BATCH_VERTEX 10000
#define MY_ALLOCATOR_BATCH_INDEX 10000

#define MY_BUFFER_ENTITY_VERTEX 0
#define MY_BUFFER_ENTITY_TRANSFORM 1

#define MY_UNIFORM_ENTITY_TEXTURE 0

#define MY_SAMPLER_ENTITY 0

#define MY_ATTRIBUTE_SPRITE_POSITION 0
#define MY_ATTRIBUTE_SPRITE_TEXTURE 1
#define MY_ATTRIBUTE_SPRITE_TRANSFORM_X 2
#define MY_ATTRIBUTE_SPRITE_TRANSFORM_Y 3
#define MY_ATTRIBUTE_SPRITE_TRANSFORM_Z 4
#define MY_ATTRIBUTE_SPRITE_TRANSFORM_W 5

#define MY_ATTRIBUTE_MESH_POSITION 0
#define MY_ATTRIBUTE_MESH_TEXTURE 1
#define MY_ATTRIBUTE_MESH_NORMAL 2
#define MY_ATTRIBUTE_MESH_TRANSFORM_X 3
#define MY_ATTRIBUTE_MESH_TRANSFORM_Y 4
#define MY_ATTRIBUTE_MESH_TRANSFORM_Z 5
#define MY_ATTRIBUTE_MESH_TRANSFORM_W 6

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////

typedef struct MyIndirect
{
    unsigned int indexCount;
    unsigned int instanceCount;
    unsigned int indexOffset;
    unsigned int vertexOffset;
    unsigned int instanceOffset;
}
MyIndirect;

typedef enum MyEntityType
{
    MY_ENTITY_TYPE_SPRITE,
    MY_ENTITY_TYPE_MESH
}
MyEntityType;

typedef struct MyEntity
{
    MyHandle entityHandle;
    MyHandle textureHandle;
    MyHandle shaderHandle;
    MyHandle batchHandle;
    GLfloat* vertices;
    GLushort* indices;
    MyEntityType type;
    MyVector position;
    MyVector scale;
    MyVector rotation;
    MyTransform transform;
    int entityIndex;
    int vertexCount;
    int vertexSize;
    int vertexOffset;
    int indexCount;
    int indexIndex;
    int frameIndex;
}
MyEntity;

typedef struct MyTextureFrame
{
    int x;
    int y;
    int width;
    int height;
}
MyTextureFrame;

typedef struct MyTexture
{
    MyHandle textureHandle;
    stbi_uc* pixels;
    MyTextureFrame* frames;
    GLuint texture;
    int width;
    int height;
    int channelCount;
    int frameCount;
    bool transparent;
}
MyTexture;

typedef struct MyShader
{
    MyHandle shaderHandle;
    GLuint vertexStage;
    GLuint fragmentStage;
    GLuint program;
    char* vertexText;
    char* fragmentText;
}
MyShader;

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

typedef struct MyBatch
{
    MyHandle batchHandle;
    MyHandle textureHandle;
    MyHandle shaderHandle;
    GLuint vertexFormat;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint transformBuffer;
    GLuint indirectBuffer;
    MyEntityType entityType;
    int entityCapacity;
    int entityCount;
    int vertexCapacity;
    int vertexCount;
    int vertexOffset;
    int indexCapacity;
    int indexCount;
}
MyBatch;

typedef struct MyEngine
{
    GLFWwindow* window;
    MyEntity* entities;
    MyTexture* textures;
    MyShader* shaders;
    MyClock* clocks;
    MyBatch* batches;
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
    int entityCapacity;
    int textureCapacity;
    int shaderCapacity;
    int clockCapacity;
    int batchCapacity;
}
MyEngine;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void my_window_position_callback(GLFWwindow* window, int x, int y);
static void my_window_size_callback(GLFWwindow* window, int width, int height);

static void my_clock_frame_callback(MyHandle clockHandle);

static MyHandle my_batch_create(MyHandle entityHandle);
static void my_batch_destroy(MyHandle batchHandle);
static MyHandle my_batch_match(MyHandle entityHandle);
static bool my_batch_add(MyHandle entityHandle);
static void my_batch_remove(MyHandle entityHandle);

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
    my_window_set_position(x, y);
    my_window_set_size(width, height);
    glfwMakeContextCurrent(myEngine.window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        my_window_destroy();
        return false;
    }
    my_window_set_color(MY_COLOR_BLACK);
    my_window_set_viewport(0.0f, 0.0f, 1.0f, 1.0f);
    my_window_set_vsync(true);
    my_window_set_depth(true);
    myEngine.entities = calloc(MY_ALLOCATOR_ENTITY, sizeof(MyEntity));
    if (!myEngine.entities)
    {
        my_window_destroy();
        return false;
    }
    myEngine.textures = calloc(MY_ALLOCATOR_TEXTURE, sizeof(MyTexture));
    if (!myEngine.textures)
    {
        my_window_destroy();
        return false;
    }
    myEngine.shaders = calloc(MY_ALLOCATOR_SHADER, sizeof(MyShader));
    if (!myEngine.shaders)
    {
        my_window_destroy();
        return false;
    }
    myEngine.clocks = calloc(MY_ALLOCATOR_CLOCK, sizeof(MyClock));
    if (!myEngine.clocks)
    {
        my_window_destroy();
        return false;
    }
    myEngine.batches = calloc(MY_ALLOCATOR_BATCH, sizeof(MyBatch));
    if (!myEngine.batches)
    {
        my_window_destroy();
        return false;
    }
    myEngine.renderMask |= GL_COLOR_BUFFER_BIT;
    myEngine.entityCapacity = MY_ALLOCATOR_ENTITY;
    myEngine.textureCapacity = MY_ALLOCATOR_TEXTURE;
    myEngine.shaderCapacity = MY_ALLOCATOR_SHADER;
    myEngine.clockCapacity = MY_ALLOCATOR_CLOCK;
    myEngine.batchCapacity = MY_ALLOCATOR_BATCH;
    stbi_set_flip_vertically_on_load(true);
    if (!my_texture_create(MY_PATH_ASSETS "/images/pixel.png", 1))
    {
        my_window_destroy();
        return false;
    }
    if (!my_shader_create(MY_PATH_ASSETS "/shaders/vertex/sprite.glsl", MY_PATH_ASSETS "/shaders/fragment/sprite.glsl"))
    {
        my_window_destroy();
        return false;
    }
    if (!my_shader_create(MY_PATH_ASSETS "/shaders/vertex/mesh.glsl", MY_PATH_ASSETS "/shaders/fragment/mesh.glsl"))
    {
        my_window_destroy();
        return false;
    }
    if (!my_clock_create())
    {
        my_window_destroy();
        return false;
    }
    my_clock_set_interval(MY_DEFAULT_CLOCK, 1.0f);
    my_clock_set_callback(MY_DEFAULT_CLOCK, my_clock_frame_callback);
    my_clock_start(MY_DEFAULT_CLOCK);
    return true;
}

void my_window_destroy(void)
{
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (myEngine.entities[i].batchHandle)
        {
            my_batch_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.entityCapacity; i++)
    {
        if (myEngine.entities[i].entityHandle)
        {
            my_entity_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.textureCapacity; i++)
    {
        if (myEngine.textures[i].textureHandle)
        {
            my_texture_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.shaderCapacity; i++)
    {
        if (myEngine.shaders[i].shaderHandle)
        {
            my_shader_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.clockCapacity; i++)
    {
        if (myEngine.clocks[i].clockHandle)
        {
            my_clock_destroy(i);
        }
    }
    if (myEngine.entities)
    {
        free(myEngine.entities);
    }
    if (myEngine.textures)
    {
        free(myEngine.textures);
    }
    if (myEngine.shaders)
    {
        free(myEngine.shaders);
    }
    if (myEngine.clocks)
    {
        free(myEngine.clocks);
    }
    if (myEngine.batches)
    {
        free(myEngine.batches);
    }
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
    glfwSwapBuffers(myEngine.window);
    glClear(myEngine.renderMask);
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
    const float windowTime = (float) glfwGetTime();
    for (int i = 1; i < myEngine.clockCapacity; i++)
    {
        if (myEngine.clocks[i].clockHandle)
        {
            const float frameTime = windowTime - myEngine.clocks[i].lastTime;
            myEngine.clocks[i].lastTime = windowTime;
            myEngine.clocks[i].totalTime += frameTime;
            myEngine.clocks[i].intervalTime += frameTime;
            if (myEngine.clocks[i].callback)
            {
                if (myEngine.clocks[i].intervalTime > myEngine.clocks[i].interval)
                {
                    myEngine.clocks[i].intervalTime = 0.0f;
                    myEngine.clocks[i].callback(i);
                }
            }
        }
    }
    myEngine.frameCount++;
    return true;
}

void my_window_render(void)
{
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (myEngine.batches[i].batchHandle)
        {
            const MyHandle shaderHandle = myEngine.batches[i].shaderHandle;
            const MyHandle textureHandle = myEngine.batches[i].textureHandle;
            glBindVertexArray(myEngine.batches[i].vertexFormat);
            glVertexArrayVertexBuffer(myEngine.batches[i].vertexFormat, MY_BUFFER_ENTITY_VERTEX, myEngine.batches[i].vertexBuffer, 0, sizeof(GLfloat) * 5);
            glVertexArrayVertexBuffer(myEngine.batches[i].vertexFormat, MY_BUFFER_ENTITY_TRANSFORM, myEngine.batches[i].transformBuffer, 0, sizeof(MyTransform));
            glVertexArrayElementBuffer(myEngine.batches[i].vertexFormat, myEngine.batches[i].indexBuffer);
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, myEngine.batches[i].indirectBuffer);
            glUseProgram(myEngine.shaders[shaderHandle].program);
            glProgramUniform1i(myEngine.shaders[shaderHandle].program, MY_UNIFORM_ENTITY_TEXTURE, 0);
            glBindTextureUnit(MY_SAMPLER_ENTITY, myEngine.textures[textureHandle].texture);
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, NULL, myEngine.batches[i].entityCount, 0);
        }
    }
}

void my_window_set_position(int x, int y)
{
    myEngine.windowX = x;
    myEngine.windowY = y;
    glfwSetWindowPos(myEngine.window, x, y);
}

void my_window_set_size(int width, int height)
{
    myEngine.windowWidth = width;
    myEngine.windowHeight = height;
    glfwSetWindowSize(myEngine.window, width, height);
}

void my_window_set_title(const char* title)
{
    glfwSetWindowTitle(myEngine.window, title);
}

void my_window_set_color(MyColor color)
{
    color = my_color_clamp(color);
    glClearColor(color.red, color.green, color.blue, color.alpha);
}

void my_window_set_viewport(float x, float y, float width, float height)
{
    myEngine.viewportX = x;
    myEngine.viewportY = y;
    myEngine.viewportWidth = width;
    myEngine.viewportHeight = height;
    glViewport(myEngine.windowWidth * x, myEngine.windowHeight * y, myEngine.windowWidth * width, myEngine.windowHeight * height);
}

void my_window_set_vsync(bool vsync)
{
    glfwSwapInterval(vsync ? 1 : 0);
}

void my_window_set_depth(bool depth)
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

void my_window_set_cursor(bool cursor)
{
    glfwSetInputMode(myEngine.window, GLFW_CURSOR, cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

float my_window_get_cursor(float* x, float* y)
{
    *x = myEngine.cursorX;
    *y = myEngine.cursorY;
}

float my_window_get_cursor_delta(float* x, float* y)
{
    *x = myEngine.cursorDeltaX;
    *y = myEngine.cursorDeltaY;
}

MyKeyState my_window_get_key_state(MyKey key)
{
    return myEngine.keyStates[key];
}

float my_window_get_time(void)
{
    return (float) glfwGetTime();
}

int my_window_get_frame_rate(void)
{
    return myEngine.frameRate;
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
    glViewport(myEngine.windowWidth * myEngine.viewportX, myEngine.windowHeight * myEngine.viewportY, width * myEngine.viewportWidth, height * myEngine.viewportHeight);
}

////////////////////////////////////////////////////////////////////////////////
// Entity Functions
////////////////////////////////////////////////////////////////////////////////

MyHandle my_entity_create_sprite(float width, float height)
{
    MyHandle entityHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.entityCapacity; i++)
    {
        if (!myEngine.entities[i].entityHandle)
        {
            entityHandle = i;
            break;
        }
    }
    if (!entityHandle)
    {
        MyEntity* entities = realloc(myEngine.entities, (myEngine.entityCapacity + MY_ALLOCATOR_ENTITY) * sizeof(MyEntity));
        if (!entities)
        {
            return MY_INVALID_HANDLE;
        }
        entityHandle = myEngine.entityCapacity;
        myEngine.entities = entities;
        myEngine.entityCapacity += MY_ALLOCATOR_ENTITY;
    }
    myEngine.entities[entityHandle].vertices = calloc(20, sizeof(float));
    if (!myEngine.entities[entityHandle].vertices)
    {
        my_entity_destroy(entityHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.entities[entityHandle].indices = calloc(6, sizeof(GLushort));
    if (!myEngine.entities[entityHandle].indices)
    {
        my_entity_destroy(entityHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.entities[entityHandle].entityHandle = entityHandle;
    myEngine.entities[entityHandle].textureHandle = MY_DEFAULT_TEXTURE;
    myEngine.entities[entityHandle].shaderHandle = MY_DEFAULT_SHADER_SPRITE;
    const GLfloat vertices[] =
    {
        -width * 0.5f, -height * 0.5f, 0.0f, 0.0f, 0.0f,
        width * 0.5f, -height * 0.5f, 0.0f, 1.0f, 0.0f,
        width * 0.5f, height * 0.5f, 0.0f, 1.0f, 1.0f,
        -width * 0.5f, height * 0.5f, 0.0f, 0.0f, 1.0f
    };
    const GLushort indices[] = { 0, 1, 2, 2, 3, 0 };
    memcpy(myEngine.entities[entityHandle].vertices, vertices, sizeof(vertices));
    memcpy(myEngine.entities[entityHandle].indices, indices, sizeof(indices));
    myEngine.entities[entityHandle].type = MY_ENTITY_TYPE_SPRITE;
    myEngine.entities[entityHandle].scale = (MyVector) { 1.0f, 1.0f, 1.0f };
    myEngine.entities[entityHandle].transform = MY_TRANSFORM_IDENTITY;
    myEngine.entities[entityHandle].vertexCount = 4;
    myEngine.entities[entityHandle].vertexSize = sizeof(GLfloat) * 5;
    myEngine.entities[entityHandle].indexCount = 6;
    return entityHandle;
}

MyHandle my_entity_create_mesh(void)
{

}

void my_entity_destroy(MyHandle entityHandle)
{
    my_entity_set_visible(entityHandle, false);
    myEngine.entities[entityHandle] = (MyEntity) { 0 };
}

void my_entity_move(MyHandle entityHandle, MyVector distance)
{
    myEngine.entities[entityHandle].position = my_vector_add(myEngine.entities[entityHandle].position, distance);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

void my_entity_scale(MyHandle entityHandle, MyVector scale)
{
    myEngine.entities[entityHandle].scale = my_vector_scale(myEngine.entities[entityHandle].scale, scale);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

void my_entity_rotate(MyHandle entityHandle, MyVector rotation)
{
    myEngine.entities[entityHandle].rotation = my_vector_add(myEngine.entities[entityHandle].rotation, rotation);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

void my_entity_set_visible(MyHandle entityHandle, bool visible)
{
    if (visible && !myEngine.entities[entityHandle].batchHandle)
    {
        my_batch_add(entityHandle);
    }
    else if (!visible && myEngine.entities[entityHandle].batchHandle)
    {
        my_batch_remove(entityHandle);
    }
}

void my_entity_set_position(MyHandle entityHandle, MyVector position)
{
    myEngine.entities[entityHandle].position = my_vector_add(myEngine.entities[entityHandle].position, position);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

void my_entity_set_scale(MyHandle entityHandle, MyVector scale)
{
    myEngine.entities[entityHandle].scale = my_vector_scale(myEngine.entities[entityHandle].scale, scale);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

void my_entity_set_rotation(MyHandle entityHandle, MyVector rotation)
{
    myEngine.entities[entityHandle].rotation = my_vector_add(myEngine.entities[entityHandle].rotation, rotation);
    myEngine.entities[entityHandle].transform = my_transform_compose(myEngine.entities[entityHandle].position, myEngine.entities[entityHandle].scale, myEngine.entities[entityHandle].rotation);
    if (myEngine.entities[entityHandle].batchHandle)
    {
        const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.entities[entityHandle].entityIndex * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    }
}

MyVector my_entity_get_position(MyHandle entityHandle)
{
    return myEngine.entities[entityHandle].position;
}

MyVector my_entity_get_scale(MyHandle entityHandle)
{
    return myEngine.entities[entityHandle].scale;
}

MyVector my_entity_get_rotation(MyHandle entityHandle)
{
    return myEngine.entities[entityHandle].rotation;
}

////////////////////////////////////////////////////////////////////////////////
// Texture Functions
////////////////////////////////////////////////////////////////////////////////

MyHandle my_texture_create(const char* imagePath, int frameCount)
{
    MyHandle textureHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.textureCapacity; i++)
    {
        if (!myEngine.textures[i].textureHandle)
        {
            textureHandle = i;
            break;
        }
    }
    if (!textureHandle)
    {
        MyTexture* textures = realloc(myEngine.textures, (myEngine.textureCapacity + MY_ALLOCATOR_TEXTURE) * sizeof(MyTexture));
        if (!textures)
        {
            return MY_INVALID_HANDLE;
        }
        textureHandle = myEngine.textureCapacity;
        myEngine.textures = textures;
        myEngine.textureCapacity += MY_ALLOCATOR_TEXTURE;
    }
    const char* extension = my_file_extension(imagePath);
    if (strcmp(extension, ".png") == 0)
    {
        myEngine.textures[textureHandle].pixels = stbi_load(imagePath, &myEngine.textures[textureHandle].width, &myEngine.textures[textureHandle].height, &myEngine.textures[textureHandle].channelCount, 0);
    }
    if (!myEngine.textures[textureHandle].pixels)
    {
        my_texture_destroy(textureHandle);
        return MY_INVALID_HANDLE;
    }
    if (myEngine.textures[textureHandle].channelCount == 4)
    {
        const int pixelsSize = myEngine.textures[textureHandle].width * myEngine.textures[textureHandle].height * 4;
        for (int i = 3; i < pixelsSize; i += 4)
        {
            if (myEngine.textures[textureHandle].pixels[i] < 255)
            {
                myEngine.textures[textureHandle].transparent = true;
                break;
            }
        }
    }
    myEngine.textures[textureHandle].frames = calloc(frameCount, sizeof(MyTextureFrame));
    if (!myEngine.textures[textureHandle].frames)
    {
        my_texture_destroy(textureHandle);
        return MY_INVALID_HANDLE;
    }
    for (int i = 0; i < frameCount; i++)
    {
        myEngine.textures[textureHandle].frames[i].width = myEngine.textures[textureHandle].width;
        myEngine.textures[textureHandle].frames[i].height = myEngine.textures[textureHandle].height;
    }
    myEngine.textures[textureHandle].frameCount = frameCount;
    glCreateTextures(GL_TEXTURE_2D, 1, &myEngine.textures[textureHandle].texture);
    if (!myEngine.textures[textureHandle].texture)
    {
        my_texture_destroy(textureHandle);
        return MY_INVALID_HANDLE;
    }
    glTextureParameteri(myEngine.textures[textureHandle].texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTextureParameteri(myEngine.textures[textureHandle].texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTextureParameteri(myEngine.textures[textureHandle].texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(myEngine.textures[textureHandle].texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(myEngine.textures[textureHandle].texture, 1, GL_RGBA8, myEngine.textures[textureHandle].width, myEngine.textures[textureHandle].height);
    glTextureSubImage2D(myEngine.textures[textureHandle].texture, 0, 0, 0, myEngine.textures[textureHandle].width, myEngine.textures[textureHandle].height, GL_RGBA, GL_UNSIGNED_BYTE, myEngine.textures[textureHandle].pixels);
    myEngine.textures[textureHandle].textureHandle = textureHandle;
    return textureHandle;
}

void my_texture_destroy(MyHandle textureHandle)
{
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (myEngine.batches[i].textureHandle == textureHandle)
        {
            my_batch_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.entityCapacity; i++)
    {
        if (myEngine.entities[i].textureHandle == textureHandle)
        {
            myEngine.entities[i].textureHandle = MY_INVALID_HANDLE;
        }
    }
    if (myEngine.textures[textureHandle].pixels)
    {
        stbi_image_free(myEngine.textures[textureHandle].pixels);
    }
    if (myEngine.textures[textureHandle].frames)
    {
        free(myEngine.textures[textureHandle].frames);
    }
    if (myEngine.textures[textureHandle].texture)
    {
        glDeleteTextures(1, &myEngine.textures[textureHandle].texture);
    }
    myEngine.textures[textureHandle] = (MyTexture) { 0 };
}

void my_texture_set_frame(MyHandle textureHandle, int frameIndex, int x, int y, int width, int height)
{
    myEngine.textures[textureHandle].frames[frameIndex] = (MyTextureFrame) { x, y, width, height };
}

////////////////////////////////////////////////////////////////////////////////
// Shader Functions
////////////////////////////////////////////////////////////////////////////////

MyHandle my_shader_create(const char* vertexPath, const char* fragmentPath)
{
    MyHandle shaderHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.shaderCapacity; i++)
    {
        if (!myEngine.shaders[i].shaderHandle)
        {
            shaderHandle = i;
            break;
        }
    }
    if (!shaderHandle)
    {
        MyShader* shaders = realloc(myEngine.shaders, (myEngine.shaderCapacity + MY_ALLOCATOR_SHADER) * sizeof(MyShader));
        if (!shaders)
        {
            return MY_INVALID_HANDLE;
        }
        shaderHandle = myEngine.shaderCapacity;
        myEngine.shaders = shaders;
        myEngine.shaderCapacity += MY_ALLOCATOR_SHADER;
    }
    myEngine.shaders[shaderHandle].vertexStage = glCreateShader(GL_VERTEX_SHADER);
    if (!myEngine.shaders[shaderHandle].vertexStage)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.shaders[shaderHandle].vertexText = my_file_read(vertexPath);
    if (!myEngine.shaders[shaderHandle].vertexText)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    const char* vertexTextConst = myEngine.shaders[shaderHandle].vertexText;
    glShaderSource(myEngine.shaders[shaderHandle].vertexStage, 1, &vertexTextConst, NULL);
    glCompileShader(myEngine.shaders[shaderHandle].vertexStage);
    GLint vertexStatus = 0;
    glGetShaderiv(myEngine.shaders[shaderHandle].vertexStage, GL_COMPILE_STATUS, &vertexStatus);
    if (!vertexStatus)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.shaders[shaderHandle].fragmentStage = glCreateShader(GL_FRAGMENT_SHADER);
    if (!myEngine.shaders[shaderHandle].fragmentStage)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.shaders[shaderHandle].fragmentText = my_file_read(fragmentPath);
    if (!myEngine.shaders[shaderHandle].fragmentText)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    const char* fragmentTextConst = myEngine.shaders[shaderHandle].fragmentText;
    glShaderSource(myEngine.shaders[shaderHandle].fragmentStage, 1, &fragmentTextConst, NULL);
    glCompileShader(myEngine.shaders[shaderHandle].fragmentStage);
    GLint fragmentStatus = 0;
    glGetShaderiv(myEngine.shaders[shaderHandle].fragmentStage, GL_COMPILE_STATUS, &fragmentStatus);
    if (!fragmentStatus)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.shaders[shaderHandle].program = glCreateProgram();
    if (!myEngine.shaders[shaderHandle].program)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    glAttachShader(myEngine.shaders[shaderHandle].program, myEngine.shaders[shaderHandle].vertexStage);
    glAttachShader(myEngine.shaders[shaderHandle].program, myEngine.shaders[shaderHandle].fragmentStage);
    glLinkProgram(myEngine.shaders[shaderHandle].program);
    GLint programStatus = 0;
    glGetProgramiv(myEngine.shaders[shaderHandle].program, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        my_shader_destroy(shaderHandle);
        return MY_INVALID_HANDLE;
    }
    myEngine.shaders[shaderHandle].shaderHandle = shaderHandle;
    return shaderHandle;
}

void my_shader_destroy(MyHandle shaderHandle)
{
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (myEngine.batches[i].shaderHandle == shaderHandle)
        {
            my_batch_destroy(i);
        }
    }
    for (int i = 1; i < myEngine.entityCapacity; i++)
    {
        if (myEngine.entities[i].shaderHandle == shaderHandle)
        {
            myEngine.entities[i].shaderHandle = MY_INVALID_HANDLE;
        }
    }
    if (myEngine.shaders[shaderHandle].vertexStage)
    {
        glDeleteShader(myEngine.shaders[shaderHandle].vertexStage);
    }
    if (myEngine.shaders[shaderHandle].fragmentStage)
    {
        glDeleteShader(myEngine.shaders[shaderHandle].fragmentStage);
    }
    if (myEngine.shaders[shaderHandle].program)
    {
        glDeleteProgram(myEngine.shaders[shaderHandle].program);
    }
    if (myEngine.shaders[shaderHandle].vertexText)
    {
        free(myEngine.shaders[shaderHandle].vertexText);
    }
    if (myEngine.shaders[shaderHandle].fragmentText)
    {
        free(myEngine.shaders[shaderHandle].fragmentText);
    }
    myEngine.shaders[shaderHandle] = (MyShader) { 0 };
}

////////////////////////////////////////////////////////////////////////////////
// Clock Functions
////////////////////////////////////////////////////////////////////////////////

MyHandle my_clock_create(void)
{
    MyHandle clockHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.clockCapacity; i++)
    {
        if (!myEngine.clocks[i].clockHandle)
        {
            clockHandle = i;
            break;
        }
    }
    if (!clockHandle)
    {
        MyClock* clocks = realloc(myEngine.clocks, (myEngine.clockCapacity + MY_ALLOCATOR_CLOCK) * sizeof(MyClock));
        if (!clocks)
        {
            return MY_INVALID_HANDLE;
        }
        clockHandle = myEngine.clockCapacity;
        myEngine.clocks = clocks;
        myEngine.clockCapacity += MY_ALLOCATOR_CLOCK;
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

void my_clock_set_interval(MyHandle clockHandle, float interval)
{
    myEngine.clocks[clockHandle].interval = interval;
    myEngine.clocks[clockHandle].intervalTime = 0.0f;
}

void my_clock_set_callback(MyHandle clockHandle, MyClockCallback callback)
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

MyVector my_vector_scale(MyVector vector, MyVector scale)
{
    return (MyVector)
    {
        vector.x * scale.x,
        vector.y * scale.y,
        vector.z * scale.z
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
    return my_vector_scale(vector, my_vector_uniform(1.0f / length));
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

MyTransform my_transform_compose(MyVector position, MyVector scale, MyVector rotation)
{
    MyTransform transform = my_transform_scale(MY_TRANSFORM_IDENTITY, scale);
    transform = my_transform_rotate(transform, rotation);
    transform = my_transform_translate(transform, position);
    return transform;
}

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
    transform.m4 += translation.x;
    transform.m8 += translation.y;
    transform.m12 += translation.z;
    return transform;
}

MyTransform my_transform_scale(MyTransform transform, MyVector scale)
{
    transform.m1 *= scale.x;
    transform.m6 *= scale.y;
    transform.m11 *= scale.z;
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

////////////////////////////////////////////////////////////////////////////////
// Batch Functions
////////////////////////////////////////////////////////////////////////////////

static MyHandle my_batch_create(MyHandle entityHandle)
{
    MyHandle batchHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (!myEngine.batches[i].batchHandle)
        {
            batchHandle = i;
            break;
        }
    }
    if (!batchHandle)
    {
        MyBatch* batches = realloc(myEngine.batches, (myEngine.batchCapacity + MY_ALLOCATOR_BATCH) * sizeof(MyBatch));
        if (!batches)
        {
            return MY_INVALID_HANDLE;
        }
        batchHandle = myEngine.batchCapacity;
        myEngine.batches = batches;
        myEngine.batchCapacity += MY_ALLOCATOR_BATCH;
    }
    glCreateVertexArrays(1, &myEngine.batches[batchHandle].vertexFormat);
    if (!myEngine.batches[batchHandle].vertexFormat)
    {
        my_batch_destroy(batchHandle);
        return MY_INVALID_HANDLE;
    }
    glCreateBuffers(1, &myEngine.batches[batchHandle].vertexBuffer);
    if (!myEngine.batches[batchHandle].vertexBuffer)
    {
        my_batch_destroy(batchHandle);
        return MY_INVALID_HANDLE;
    }
    glCreateBuffers(1, &myEngine.batches[batchHandle].indexBuffer);
    if (!myEngine.batches[batchHandle].indexBuffer)
    {
        my_batch_destroy(batchHandle);
        return MY_INVALID_HANDLE;
    }
    glCreateBuffers(1, &myEngine.batches[batchHandle].transformBuffer);
    if (!myEngine.batches[batchHandle].transformBuffer)
    {
        my_batch_destroy(batchHandle);
        return MY_INVALID_HANDLE;
    }
    glCreateBuffers(1, &myEngine.batches[batchHandle].indirectBuffer);
    if (!myEngine.batches[batchHandle].indirectBuffer)
    {
        my_batch_destroy(batchHandle);
        return MY_INVALID_HANDLE;
    }
    if (myEngine.batches[batchHandle].entityType == MY_ENTITY_TYPE_SPRITE)
    {
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_POSITION, MY_BUFFER_ENTITY_VERTEX);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TEXTURE, MY_BUFFER_ENTITY_VERTEX);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_X, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Y, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Z, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_W, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_POSITION, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_X, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Y, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Z, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_W, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 12);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_POSITION);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TEXTURE);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_X);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Y);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_Z);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_SPRITE_TRANSFORM_W);
        glVertexArrayBindingDivisor(myEngine.batches[batchHandle].vertexFormat, MY_BUFFER_ENTITY_TRANSFORM, 1);
    }
    else if (myEngine.batches[batchHandle].entityType == MY_ENTITY_TYPE_MESH)
    {
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_POSITION, MY_BUFFER_ENTITY_VERTEX);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TEXTURE, MY_BUFFER_ENTITY_VERTEX);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_NORMAL, MY_BUFFER_ENTITY_VERTEX);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_X, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Y, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Z, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribBinding(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_W, MY_BUFFER_ENTITY_TRANSFORM);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_POSITION, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_X, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Y, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Z, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8);
        glVertexArrayAttribFormat(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_W, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 12);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_POSITION);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TEXTURE);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_NORMAL);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_X);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Y);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_Z);
        glEnableVertexArrayAttrib(myEngine.batches[batchHandle].vertexFormat, MY_ATTRIBUTE_MESH_TRANSFORM_W);
        glVertexArrayBindingDivisor(myEngine.batches[batchHandle].vertexFormat, MY_BUFFER_ENTITY_TRANSFORM, 1);
    }
    glNamedBufferStorage(myEngine.batches[batchHandle].vertexBuffer, MY_ALLOCATOR_BATCH_VERTEX, NULL, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(myEngine.batches[batchHandle].indexBuffer, MY_ALLOCATOR_BATCH_INDEX, NULL, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(myEngine.batches[batchHandle].transformBuffer, MY_ALLOCATOR_BATCH_ENTITY * sizeof(MyTransform), NULL, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(myEngine.batches[batchHandle].indirectBuffer, MY_ALLOCATOR_BATCH_ENTITY * sizeof(MyIndirect), NULL, GL_DYNAMIC_STORAGE_BIT);
    myEngine.batches[batchHandle].batchHandle = batchHandle;
    myEngine.batches[batchHandle].textureHandle = myEngine.entities[entityHandle].textureHandle;
    myEngine.batches[batchHandle].shaderHandle = myEngine.entities[entityHandle].shaderHandle;
    myEngine.batches[batchHandle].entityType = myEngine.entities[entityHandle].type;
    myEngine.batches[batchHandle].entityCapacity = MY_ALLOCATOR_BATCH_ENTITY;
    myEngine.batches[batchHandle].vertexCapacity = MY_ALLOCATOR_BATCH_VERTEX;
    myEngine.batches[batchHandle].indexCapacity = MY_ALLOCATOR_BATCH_INDEX;
    return batchHandle;
}

static void my_batch_destroy(MyHandle batchHandle)
{
    for (int i = 1; i < myEngine.entityCapacity; i++)
    {
        if (myEngine.entities[i].batchHandle == batchHandle)
        {
            myEngine.entities[i].batchHandle = MY_INVALID_HANDLE;
            myEngine.entities[i].entityIndex = 0;
            myEngine.entities[i].vertexOffset = 0;
            myEngine.entities[i].indexIndex = 0;
        }
    }
    if (myEngine.batches[batchHandle].vertexFormat)
    {
        glDeleteVertexArrays(1, &myEngine.batches[batchHandle].vertexFormat);
    }
    if (myEngine.batches[batchHandle].vertexBuffer)
    {
        glDeleteBuffers(1, &myEngine.batches[batchHandle].vertexBuffer);
    }
    if (myEngine.batches[batchHandle].indexBuffer)
    {
        glDeleteBuffers(1, &myEngine.batches[batchHandle].indexBuffer);
    }
    if (myEngine.batches[batchHandle].transformBuffer)
    {
        glDeleteBuffers(1, &myEngine.batches[batchHandle].transformBuffer);
    }
    if (myEngine.batches[batchHandle].indirectBuffer)
    {
        glDeleteBuffers(1, &myEngine.batches[batchHandle].indirectBuffer);
    }
    myEngine.batches[batchHandle] = (MyBatch) { 0 };
}

static MyHandle my_batch_match(MyHandle entityHandle)
{
    MyHandle batchHandle = MY_INVALID_HANDLE;
    for (int i = 1; i < myEngine.batchCapacity; i++)
    {
        if (myEngine.batches[i].batchHandle &&
            myEngine.batches[i].textureHandle == myEngine.entities[entityHandle].textureHandle &&
            myEngine.batches[i].shaderHandle == myEngine.entities[entityHandle].shaderHandle)
        {
            return i;
        }
    }
    return MY_INVALID_HANDLE;
}

static bool my_batch_add(MyHandle entityHandle)
{
    MyHandle batchHandle = my_batch_match(entityHandle);
    if (!batchHandle)
    {
        batchHandle = my_batch_create(entityHandle);
        if (!batchHandle)
        {
            return false;
        }
    }
    if (myEngine.batches[batchHandle].entityCount + 1 > myEngine.batches[batchHandle].entityCapacity)
    {
        GLuint transformBuffer;
        GLuint indirectBuffer;
        glCreateBuffers(1, &transformBuffer);
        glCreateBuffers(1, &indirectBuffer);
        if (!transformBuffer || !indirectBuffer)
        {
            return false;
        }
        glNamedBufferStorage(myEngine.batches[batchHandle].transformBuffer, (myEngine.batches[batchHandle].entityCapacity + MY_ALLOCATOR_BATCH_ENTITY) * sizeof(MyTransform), NULL, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferStorage(myEngine.batches[batchHandle].indirectBuffer, (myEngine.batches[batchHandle].entityCapacity + MY_ALLOCATOR_BATCH_ENTITY) * sizeof(MyIndirect), NULL, GL_DYNAMIC_STORAGE_BIT);
        glCopyNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, transformBuffer, 0, 0, myEngine.batches[batchHandle].entityCapacity * sizeof(MyTransform));
        glCopyNamedBufferSubData(myEngine.batches[batchHandle].indirectBuffer, indirectBuffer, 0, 0, myEngine.batches[batchHandle].entityCapacity * sizeof(MyIndirect));
        glDeleteBuffers(1, &myEngine.batches[batchHandle].transformBuffer);
        glDeleteBuffers(1, &myEngine.batches[batchHandle].indirectBuffer);
        myEngine.batches[batchHandle].transformBuffer = transformBuffer;
        myEngine.batches[batchHandle].indirectBuffer = indirectBuffer;
    }
    // if (myEngine.batches[batchHandle].vertexCount + myEngine.entities[entityHandle].vertexCount > myEngine.batches[batchHandle].vertexCapacity)
    // {
    //     GLuint vertexBuffer;
    //     glCreateBuffers(1, &vertexBuffer);
    //     if (!vertexBuffer)
    //     {
    //         my_batch_destroy(batchHandle);
    //         return false;
    //     }
    //     glNamedBufferStorage(myEngine.batches[batchHandle].vertexBuffer, (myEngine.batches[batchHandle].vertexCapacity + MY_ALLOCATOR_BATCH_ENTITY) * sizeof(MyEntityVertex) * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
    //     glCopyNamedBufferSubData(myEngine.batches[batchHandle].vertexBuffer, vertexBuffer, 0, 0, myEngine.batches[batchHandle].vertexCapacity * sizeof(MyEntityVertex) * 4);
    //     glDeleteBuffers(1, &myEngine.batches[batchHandle].vertexBuffer);
    //     myEngine.batches[batchHandle].vertexBuffer = vertexBuffer;
    // }
    // if (myEngine.batches[batchHandle].indexCount + 6 > myEngine.batches[batchHandle].indexCapacity)
    // {
    //     GLuint indexBuffer;
    //     glCreateBuffers(1, &indexBuffer);
    //     if (!indexBuffer)
    //     {
    //         my_batch_destroy(batchHandle);
    //         return false;
    //     }
    //     glNamedBufferStorage(myEngine.batches[batchHandle].indexBuffer, (myEngine.batches[batchHandle].indexCapacity + MY_ALLOCATOR_BATCH_ENTITY) * sizeof(GLushort) * 6, NULL, GL_DYNAMIC_STORAGE_BIT);
    //     glCopyNamedBufferSubData(myEngine.batches[batchHandle].indexBuffer, indexBuffer, 0, 0, myEngine.batches[batchHandle].indexCapacity * sizeof(GLushort) * 6);
    //     glDeleteBuffers(1, &myEngine.batches[batchHandle].indexBuffer);
    //     myEngine.batches[batchHandle].indexBuffer = indexBuffer;
    // }
    glNamedBufferSubData(myEngine.batches[batchHandle].vertexBuffer, myEngine.batches[batchHandle].vertexOffset, myEngine.entities[entityHandle].vertexCount * myEngine.entities[entityHandle].vertexSize, myEngine.entities[entityHandle].vertices);
    glNamedBufferSubData(myEngine.batches[batchHandle].indexBuffer, myEngine.batches[batchHandle].indexCount * sizeof(GLushort), myEngine.entities[entityHandle].indexCount * sizeof(GLushort), myEngine.entities[entityHandle].indices);
    glNamedBufferSubData(myEngine.batches[batchHandle].transformBuffer, myEngine.batches[batchHandle].entityCount * sizeof(MyTransform), sizeof(MyTransform), &myEngine.entities[entityHandle].transform);
    const MyIndirect indirect =
    {
        myEngine.entities[entityHandle].indexCount,
        1,
        myEngine.batches[batchHandle].indexCount,
        myEngine.batches[batchHandle].vertexCount,
        myEngine.batches[batchHandle].entityCount
    };
    glNamedBufferSubData(myEngine.batches[batchHandle].indirectBuffer, myEngine.batches[batchHandle].entityCount * sizeof(MyIndirect), sizeof(MyIndirect), &indirect);
    myEngine.entities[entityHandle].batchHandle = batchHandle;
    myEngine.entities[entityHandle].entityIndex = myEngine.batches[batchHandle].entityCount;
    myEngine.entities[entityHandle].vertexOffset = myEngine.batches[batchHandle].vertexOffset;
    myEngine.entities[entityHandle].indexIndex = myEngine.batches[batchHandle].indexCount;
    myEngine.batches[batchHandle].entityCount++;
    myEngine.batches[batchHandle].vertexCount += myEngine.entities[entityHandle].vertexCount;
    myEngine.batches[batchHandle].vertexOffset += myEngine.entities[entityHandle].vertexCount * myEngine.entities[entityHandle].vertexSize;
    myEngine.batches[batchHandle].indexCount += myEngine.entities[entityHandle].indexCount;
    return true;
}

static void my_batch_remove(MyHandle entityHandle)
{
    const MyHandle batchHandle = myEngine.entities[entityHandle].batchHandle;
        // todo: swap deleted sprite with last in buffers
    myEngine.entities[entityHandle].batchHandle = MY_INVALID_HANDLE;
    myEngine.entities[entityHandle].entityIndex = 0;
    myEngine.entities[entityHandle].vertexOffset = 0;
    myEngine.entities[entityHandle].indexCount = 0;
    myEngine.batches[batchHandle].entityCount--;
    myEngine.batches[batchHandle].vertexCount -= myEngine.entities[entityHandle].vertexCount;
    myEngine.batches[batchHandle].vertexOffset -= myEngine.entities[entityHandle].vertexCount * myEngine.entities[entityHandle].vertexSize;
    myEngine.batches[batchHandle].indexCount-= myEngine.entities[entityHandle].indexCount;
    if (!myEngine.batches[batchHandle].entityCount)
    {
        my_batch_destroy(batchHandle);
    }
}