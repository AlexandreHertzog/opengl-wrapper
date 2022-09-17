add_library(game-engine-factories program_factory.cpp shape_factory.cpp texture_factory.cpp)
target_link_libraries(game-engine-factories PUBLIC opengl-cpp PRIVATE game-engine-data-types)