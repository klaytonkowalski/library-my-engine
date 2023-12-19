################################################################################
# Initialize stb_image.

project(stb_image LANGUAGES C)

################################################################################
# Set stb_image variables.

set(STB_IMAGE_PATH_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(STB_IMAGE_FILES_HEADERS
    ${STB_IMAGE_PATH_HEADERS}/stb_image/stb_image.h)

################################################################################
# Build stb_image.

add_library(stb_image INTERFACE)

target_include_directories(stb_image
    INTERFACE
    $<BUILD_INTERFACE:${STB_IMAGE_PATH_HEADERS}>)