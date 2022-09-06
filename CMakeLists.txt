cmake_minimum_required(VERSION 3.22)

project(game-engine VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost COMPONENTS log REQUIRED)

option(INCLUDE_EXAMPLES "Build the examples contained on ./examples/" ON)

add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)

add_subdirectory(lib)

add_library(game-engine-utils
        src/utils/exception.cpp
        )

target_include_directories(game-engine-utils
        PUBLIC include
        PRIVATE include/game-engine/utils
        )

target_link_libraries(game-engine-utils
        PRIVATE Boost::log backtrace
        )

add_library(game-engine-data-types
        src/data_types/camera.cpp
        src/data_types/face.cpp
        src/data_types/image.cpp
        src/data_types/mesh.cpp
        src/data_types/shape.cpp
        src/data_types/window.cpp
        )

target_include_directories(game-engine-data-types
        PUBLIC include
        PRIVATE include/game-engine include/game-engine/data_types
        )

target_link_libraries(game-engine-data-types
        PUBLIC glm game-engine-utils opengl-cpp
        PRIVATE game-engine-utils stb game-engine-parsers Boost::log
        )

add_library(game-engine-parsers
        src/parsers/obj_parser.cpp)

target_include_directories(game-engine-parsers
        PUBLIC include
        PRIVATE include/game-engine include/game-engine/parsers
        )

target_link_libraries(game-engine-parsers
        PUBLIC glm
        PRIVATE game-engine-utils
        )

add_library(game-engine INTERFACE)
target_include_directories(game-engine PUBLIC INTERFACE include)
target_link_libraries(game-engine PUBLIC INTERFACE game-engine-data-types game-engine-parsers opengl-cpp)

add_executable(game-engine-test src/main.cpp src/integration.cpp src/integration.h)
target_link_libraries(game-engine-test PRIVATE game-engine Boost::log glm imgui)

add_subdirectory(test)
