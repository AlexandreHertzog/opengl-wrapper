cmake_minimum_required(VERSION 3.22)

project(opengl-wrapper VERSION 0.1.0 LANGUAGES C CXX)

find_package(glfw3 REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

option(INCLUDE_EXAMPLES "Build the examples contained on ./examples/" ON)

add_subdirectory(lib)

add_library(opengl-wrapper-exceptions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions/gl_error.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions/glad_error.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions/glfw_error.cpp
)

target_include_directories(opengl-wrapper-exceptions
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl-wrapper/exceptions
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(opengl-wrapper-exceptions
    PRIVATE Boost::log
)

add_library(opengl-wrapper-utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/formatter.cpp
)

add_library(opengl-wrapper-wraps
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wraps/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wraps/program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wraps/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wraps/vertex_arrays.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/wraps/window.cpp
)

target_include_directories(opengl-wrapper-wraps
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl-wrapper/wraps
            ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl-wrapper
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(opengl-wrapper-wraps
        PUBLIC glad opengl-wrapper-exceptions
        PRIVATE Boost::log
        )

add_library(opengl-wrapper
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/window_manager.cpp
)

target_include_directories(opengl-wrapper
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl-wrapper
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(opengl-wrapper
    PUBLIC glad glfw opengl-wrapper-exceptions opengl-wrapper-wraps
    PRIVATE opengl-wrapper-utils Boost::log
)

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif()