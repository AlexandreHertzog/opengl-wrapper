include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(data_types)
add_subdirectory(parsers)
add_subdirectory(utils)

add_library(game-engine integration.cpp renderer.cpp)
target_include_directories(game-engine PUBLIC src)
target_link_libraries(game-engine PUBLIC game-engine-data-types game-engine-parsers opengl-cpp PRIVATE imgui)

add_executable(game-engine-test main.cpp)
target_link_libraries(game-engine-test PRIVATE game-engine)
