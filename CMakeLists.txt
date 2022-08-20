cmake_minimum_required(VERSION 3.22)

project(opengl-wrapper VERSION 0.1.0 LANGUAGES C CXX)

find_package(glfw3 REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

option(INCLUDE_EXAMPLES "Build the examples contained on ./examples/" ON)

add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)

add_subdirectory(lib)

add_library(opengl-wrapper-utils
        src/utils/exception.cpp
        src/utils/formatter.cpp
        src/utils/gl_error.cpp
        src/utils/glad_error.cpp
        src/utils/glfw_error.cpp
        )

target_include_directories(opengl-wrapper-utils
        PUBLIC include
        PRIVATE include/opengl-wrapper/utils
        )

target_link_libraries(opengl-wrapper-utils
        PRIVATE Boost::log backtrace
        )

add_library(opengl-wrapper-graphics
        src/graphics/graphics.cpp
        )

target_include_directories(opengl-wrapper-graphics
        PUBLIC include
        PRIVATE include/opengl-wrapper/graphics
        )

target_link_libraries(opengl-wrapper-graphics
        PUBLIC glad glfw
        PRIVATE Boost::log
        )

add_library(opengl-wrapper-textures
        src/textures/texture.cpp
        src/textures/texture_controller.cpp
        src/textures/image.cpp
        )

target_include_directories(opengl-wrapper-textures
        PUBLIC include
        PRIVATE include/opengl-wrapper/textures
        )

target_link_libraries(opengl-wrapper-textures
        PRIVATE opengl-wrapper-graphics stb opengl-wrapper-utils
        )

add_library(opengl-wrapper-models
        src/models/vertex.cpp
        src/models/shape.cpp
        )

target_include_directories(opengl-wrapper-models
        PUBLIC include
        PRIVATE include/opengl-wrapper/models
        )

target_link_libraries(opengl-wrapper-models
        PRIVATE opengl-wrapper-graphics
        )

add_library(opengl-wrapper-wraps
        src/wraps/buffer.cpp
        src/wraps/program.cpp
        src/wraps/shader.cpp
        src/wraps/vertex_array.cpp
        src/wraps/window.cpp
        src/wraps/buffer_controller.cpp
        src/wraps/vertex_array_controller.cpp
        )

target_include_directories(opengl-wrapper-wraps
        PUBLIC include
        PRIVATE include/opengl-wrapper/wraps include/opengl-wrapper
        )

target_link_libraries(opengl-wrapper-wraps
        PUBLIC glad opengl-wrapper-utils
        PRIVATE Boost::log
        )

add_library(opengl-wrapper
        src/gl_manager.cpp
        src/renderer.cpp
        src/window_manager.cpp
        )

target_include_directories(opengl-wrapper
        PUBLIC include
        PRIVATE include/opengl-wrapper
        )

target_link_libraries(opengl-wrapper
        PUBLIC glad glfw opengl-wrapper-wraps opengl-wrapper-models opengl-wrapper-textures
        PRIVATE opengl-wrapper-utils Boost::log
        )

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif ()