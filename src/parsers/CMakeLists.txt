add_library(game-engine-parsers obj_parser.cpp)
target_link_libraries(game-engine-parsers PUBLIC glm PRIVATE game-engine-utils)
