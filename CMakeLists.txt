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

add_library(opengl-wrapper-data-types
        src/data_types/camera.cpp
        src/data_types/face.cpp
        src/data_types/image.cpp
        src/data_types/mesh.cpp
        src/data_types/shape.cpp
        src/data_types/window.cpp
        )

target_include_directories(opengl-wrapper-data-types
        PUBLIC include
        PRIVATE include/opengl-wrapper include/opengl-wrapper/data_types
        )

target_link_libraries(opengl-wrapper-data-types
        PUBLIC glm opengl-wrapper-utils opengl-cpp
        PRIVATE opengl-wrapper-utils stb opengl-wrapper-parsers Boost::log
        )

add_library(opengl-wrapper-parsers
        src/parsers/obj_parser.cpp)

target_include_directories(opengl-wrapper-parsers
        PUBLIC include
        PRIVATE include/opengl-wrapper include/opengl-wrapper/parsers
        )

target_link_libraries(opengl-wrapper-parsers
        PUBLIC glm
        PRIVATE opengl-wrapper-utils
        )

add_library(opengl-wrapper INTERFACE)
target_include_directories(opengl-wrapper PUBLIC INTERFACE include)
target_link_libraries(opengl-wrapper PUBLIC INTERFACE opengl-wrapper-data-types opengl-wrapper-parsers)

if (INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif ()

add_subdirectory(test)