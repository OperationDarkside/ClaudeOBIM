cmake_minimum_required(VERSION 3.15)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Verify GLAD files exist
set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/src/glad.c")
if(NOT EXISTS ${GLAD_SOURCE})
    message(FATAL_ERROR "GLAD source file not found at ${GLAD_SOURCE}")
endif()

# Vcpkg toolchain file integration
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLM REQUIRED)

# Add glad as a static library
add_library(glad STATIC
    ${GLAD_SOURCE}
)

target_include_directories(glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
)

# STB is header-only
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb)

add_executable(image_processor 
    src/main.cpp
    src/shader.cpp
    src/texture.cpp
    src/gl_context.cpp
)

target_include_directories(image_processor PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/stb
)

target_link_libraries(image_processor PRIVATE 
    OpenGL::GL
    glfw
    glm::glm
    glad
    stb
)