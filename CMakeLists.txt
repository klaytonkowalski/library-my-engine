################################################################################
# Initialize myengine.

cmake_minimum_required(VERSION 3.14)

project(myengine
    VERSION 0.0.0
    DESCRIPTION "My Engine is a game engine that makes everything easy."
    HOMEPAGE_URL "https://github.com/klaytonkowalski/library-my-engine"
    LANGUAGES C)

################################################################################
# Include CMake modules.

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)
include(MyOptions)

################################################################################
# Set CMake variables.

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

################################################################################
# Set myengine variables.

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MYENGINE_STANDALONE ON)
else()
    set(MYENGINE_STANDALONE OFF)
endif()

set(MYENGINE_PATH_ASSETS ${CMAKE_CURRENT_LIST_DIR}/assets)
set(MYENGINE_PATH_EXAMPLES ${CMAKE_CURRENT_LIST_DIR}/examples)
set(MYENGINE_PATH_EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/external)
set(MYENGINE_PATH_GLAD ${MYENGINE_PATH_EXTERNAL}/glad)
set(MYENGINE_PATH_GLFW ${MYENGINE_PATH_EXTERNAL}/glfw)
set(MYENGINE_PATH_STB_IMAGE ${MYENGINE_PATH_EXTERNAL}/stb_image)
set(MYENGINE_PATH_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include)
set(MYENGINE_PATH_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src)

set(MYENGINE_FILES_HEADERS
    ${MYENGINE_PATH_HEADERS}/myengine.h)
set(MYENGINE_FILES_SOURCES
    ${MYENGINE_PATH_SOURCES}/myengine.c)

################################################################################
# Set library variables.

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

################################################################################
# Build libraries.

add_subdirectory(${MYENGINE_PATH_GLAD})
add_subdirectory(${MYENGINE_PATH_GLFW})
add_subdirectory(${MYENGINE_PATH_STB_IMAGE})

################################################################################
# Build myengine.

add_library(myengine STATIC ${MYENGINE_FILES_SOURCES})

target_compile_definitions(myengine
    PRIVATE
    MYENGINE_EXPORT
    PUBLIC
    $<BUILD_INTERFACE:MYENGINE_PATH_ASSETS=\"${MYENGINE_PATH_ASSETS}\">)

target_include_directories(myengine
    PUBLIC
    $<BUILD_INTERFACE:${MYENGINE_PATH_HEADERS}>)

target_link_libraries(myengine
    PUBLIC
    glad
    glfw
    stb_image)

################################################################################
# Build examples.

add_executable(example ${MYENGINE_PATH_EXAMPLES}/example/example.c)

target_link_libraries(example PRIVATE myengine)