cmake_minimum_required(VERSION 3.22)

project(game-engine VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost COMPONENTS log REQUIRED)
add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
