# Copyright (c) 2023 Klayton Kowalski
# 
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
# 
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
# 
# 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
# 
# 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
# 
# 3. This notice may not be removed or altered from any source distribution.

################################################################################
# Welcome to the My Engine build system!
# Let's start by initializing the project.

cmake_minimum_required(VERSION 3.14)

project(myengine
    VERSION 0.0.0
    DESCRIPTION "My Engine is a game engine that makes everything easy."
    HOMEPAGE_URL "https://github.com/klaytonkowalski/library-my-engine"
    LANGUAGES C)

################################################################################
# CMake has some really important variables that we have to configure.
# Some of those variables come from CMake's "standard library" so-to-speak, so let's also include those modules.

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)

################################################################################
# Now let's get to the My Engine-specific stuff.
# Lots of convenience variables are defined here, along with variables in "MyOptions.cmake" which should be configured by users.

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MYENGINE_STANDALONE ON)
else()
    set(MYENGINE_STANDALONE OFF)
endif()

set(MYENGINE_PATH_ASSETS ${CMAKE_CURRENT_LIST_DIR}/assets)
set(MYENGINE_PATH_EXAMPLES ${CMAKE_CURRENT_LIST_DIR}/examples)
set(MYENGINE_PATH_EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/external)
set(MYENGINE_PATH_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include)
set(MYENGINE_PATH_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src)

set(MYENGINE_FILES_HEADERS
    ${MYENGINE_PATH_HEADERS}/myengine/export.h
    ${MYENGINE_PATH_HEADERS}/myengine/myengine.h)
set(MYENGINE_FILES_SOURCES
    ${MYENGINE_PATH_SOURCES}/myengine.c)

include(MyOptions)

################################################################################
# Before we can build My Engine, we need to deal with all the third-party libraries it uses.
# Some of them do not use CMake, so let's create a CMake build system for them!
# We will start by initializing their projects.

project(glad LANGUAGES C)
project(stb_image LANGUAGES C)

################################################################################
# Next, let's define some convenience variables.
# The libraries that do use CMake require us to define some configuration variables, so let's do that too.

set(GLAD_PATH_HEADERS ${MYENGINE_PATH_EXTERNAL}/glad/include)
set(GLAD_PATH_SOURCES ${MYENGINE_PATH_EXTERNAL}/glad/src)
set(GLFW_PATH ${MYENGINE_PATH_EXTERNAL}/glfw)
set(STB_IMAGE_PATH_HEADERS ${MYENGINE_PATH_EXTERNAL}/stb_image/include)

set(GLAD_FILES_HEADERS
    ${GLAD_PATH_HEADERS}/glad/gl.h
    ${GLAD_PATH_HEADERS}/glad/khrplatform.h)
set(GLAD_FILES_SOURCES
    ${GLAD_PATH_SOURCES}/gl.c)
set(STB_IMAGE_FILES_HEADERS
    ${STB_IMAGE_PATH_HEADERS}/std_image/stb_image.h)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

################################################################################
# Finally, let's build them.
# We want to build dependencies directly into My Engine, so we mark them as static libraries.
# Header-only libraries are marked as interface because they're not actually compiled, they're just copy and pasted in My Engine.

add_library(glad STATIC ${GLAD_FILES_SOURCES})
add_library(stb_image INTERFACE)

target_include_directories(glad
    PUBLIC
    $<BUILD_INTERFACE:${GLAD_PATH_HEADERS}>)
target_include_directories(stb_image
    INTERFACE
    $<BUILD_INTERFACE:${STB_IMAGE_PATH_HEADERS}>)

add_subdirectory(${GLFW_PATH})

################################################################################
# Now that all of the libraries are ready to go, let's build My Engine.

add_library(myengine STATIC ${MYENGINE_FILES_SOURCES})

target_link_libraries(myengine
    PUBLIC
    glad
    glfw
    stb_image)

target_include_directories(myengine
    PUBLIC
    $<BUILD_INTERFACE:${MYENGINE_PATH_HEADERS}>)

target_compile_definitions(myengine
    PRIVATE
    MYENGINE_EXPORT
    $<BUILD_INTERFACE:MYENGINE_PATH_ASSETS=\"${MYENGINE_PATH_ASSETS}\">)

################################################################################
# Example projects are very important for demonstrating how to use My Engine.
# The user may not be interested in them though, so let's only build them if option is set.

if(MYENGINE_OPTION_EXAMPLES)
    add_executable(example ${MYENGINE_PATH_EXAMPLES}/example/example.c)
    target_link_libraries(example PRIVATE myengine)
endif()