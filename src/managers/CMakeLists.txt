add_library(game-engine-managers light_manager.cpp)
target_link_libraries(game-engine-managers PUBLIC opengl-cpp PRIVATE game-engine-data-types)