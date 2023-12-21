################################################################################
# License
################################################################################

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

cmake_minimum_required(VERSION 3.14)

project(myengine
    VERSION 0.0.0
    DESCRIPTION "My Engine is a game engine that makes everything easy."
    HOMEPAGE_URL "https://github.com/klaytonkowalski/library-my-engine"
    LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MY_STANDALONE ON)
else()
    set(MY_STANDALONE OFF)
endif()
set(MY_PATH_ASSETS ${CMAKE_CURRENT_LIST_DIR}/assets)
set(MY_PATH_EXAMPLES ${CMAKE_CURRENT_LIST_DIR}/examples)
set(MY_PATH_EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/external)
set(MY_PATH_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include)
set(MY_PATH_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src)
set(MY_FILES_HEADERS
    ${MY_PATH_HEADERS}/myengine/export.h
    ${MY_PATH_HEADERS}/myengine/myengine.h)
set(MY_FILES_SOURCES
    ${MY_PATH_SOURCES}/myengine.c)

set(GLAD_PATH_HEADERS ${MY_PATH_EXTERNAL}/glad/include)
set(GLAD_PATH_SOURCES ${MY_PATH_EXTERNAL}/glad/src)
set(GLFW_PATH ${MY_PATH_EXTERNAL}/glfw)
set(STB_IMAGE_PATH_HEADERS ${MY_PATH_EXTERNAL}/stb_image/include)

set(GLAD_FILES_HEADERS
    ${GLAD_PATH_HEADERS}/glad/gl.h
    ${GLAD_PATH_HEADERS}/glad/khrplatform.h)
set(GLAD_FILES_SOURCES
    ${GLAD_PATH_SOURCES}/gl.c)
set(STB_IMAGE_FILES_HEADERS
    ${STB_IMAGE_PATH_HEADERS}/std_image/stb_image.h)

include(GNUInstallDirs)
include(MyOptions)

project(glad LANGUAGES C)
project(stb_image LANGUAGES C)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_library(glad STATIC ${GLAD_FILES_SOURCES})
add_library(stb_image INTERFACE)

target_include_directories(glad
    PUBLIC
    $<BUILD_INTERFACE:${GLAD_PATH_HEADERS}>)
target_include_directories(stb_image
    INTERFACE
    $<BUILD_INTERFACE:${STB_IMAGE_PATH_HEADERS}>)

add_subdirectory(${GLFW_PATH})

add_library(myengine STATIC ${MY_FILES_SOURCES})

target_link_libraries(myengine
    PUBLIC
    glad
    glfw
    stb_image)

target_include_directories(myengine
    PUBLIC
    $<BUILD_INTERFACE:${MY_PATH_HEADERS}>)

target_compile_definitions(myengine
    PRIVATE
    MY_EXPORT
    MY_OPTION_CAPACITY_CLOCK=${MY_OPTION_CAPACITY_CLOCK}
    $<BUILD_INTERFACE:MY_PATH_ASSETS=\"${MY_PATH_ASSETS}\">)

if(MY_OPTION_EXAMPLES)
    add_executable(example ${MY_PATH_EXAMPLES}/example/example.c)
    target_link_libraries(example PRIVATE myengine)
endif()