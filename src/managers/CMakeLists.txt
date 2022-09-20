add_library(game-engine-managers light_manager.cpp shape_manager.cpp)
target_link_libraries(game-engine-managers PUBLIC opengl-cpp game-engine-factories PRIVATE game-engine-data-types)