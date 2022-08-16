cmake_minimum_required(VERSION 3.22)

project(opengl-wrapper VERSION 0.1.0 LANGUAGES C CXX)

find_package(glfw3 REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

option(INCLUDE_EXAMPLES "Build the examples contained on ./examples/" ON)

add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)

add_subdirectory(lib)

add_library(opengl-wrapper-exceptions
        src/exceptions/exception.cpp
        src/exceptions/gl_error.cpp
        src/exceptions/glad_error.cpp
        src/exceptions/glfw_error.cpp
        )

target_include_directories(opengl-wrapper-exceptions
        PUBLIC include
        PRIVATE include/opengl-wrapper/exceptions src
        )

target_link_libraries(opengl-wrapper-exceptions
        PRIVATE Boost::log backtrace
        )

add_library(opengl-wrapper-models
        src/models/vertex.cpp
        src/models/shape.cpp
        )

target_include_directories(opengl-wrapper-models
        PUBLIC include
        PRIVATE include/opengl-wrapper/models src
        )

add_library(opengl-wrapper-wraps
        src/wraps/buffer.cpp
        src/wraps/program.cpp
        src/wraps/shader.cpp
        src/wraps/vertex_arrays.cpp
        src/wraps/window.cpp
        src/wraps/api.cpp
        )

target_include_directories(opengl-wrapper-wraps
        PUBLIC include
        PRIVATE include/opengl-wrapper/wraps include/opengl-wrapper src
        )

target_link_libraries(opengl-wrapper-wraps
        PUBLIC glad opengl-wrapper-exceptions
        PRIVATE Boost::log
        )

add_library(opengl-wrapper-utils
        src/utils/formatter.cpp
        )

target_include_directories(opengl-wrapper-utils
        PUBLIC include
        PRIVATE include/opengl-wrapper/utils src
        )

target_link_libraries(opengl-wrapper-utils
        PRIVATE Boost::log opengl-wrapper-wraps
        )

add_library(opengl-wrapper
        src/gl_manager.cpp
        src/renderer.cpp
        src/window_manager.cpp
        )

target_include_directories(opengl-wrapper
        PUBLIC include
        PRIVATE include/opengl-wrapper src
        )

target_link_libraries(opengl-wrapper
        PUBLIC glad glfw opengl-wrapper-exceptions opengl-wrapper-wraps opengl-wrapper-models
        PRIVATE opengl-wrapper-utils Boost::log
        )

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif ()