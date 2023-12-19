################################################################################
# Initialize glad.

project(glad LANGUAGES C)

################################################################################
# Set glad variables.

set(GLAD_PATH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GLAD_PATH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(GLAD_FILES_HEADERS
    ${GLAD_PATH_HEADERS}/glad/gl.h
    ${GLAD_PATH_HEADERS}/glad/khrplatform.h)
set(GLAD_FILES_SOURCES
    ${GLAD_PATH_SOURCES}/gl.c)

################################################################################
# Build glad.

add_library(glad STATIC ${GLAD_FILES_SOURCES})

target_include_directories(glad
    PUBLIC
    $<BUILD_INTERFACE:${GLAD_PATH_HEADERS}>)