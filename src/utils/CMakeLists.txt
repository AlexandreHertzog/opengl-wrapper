add_library(game-engine-utils exception.cpp program_shapes.cpp)
target_link_libraries(game-engine-utils PUBLIC game-engine-data-types PRIVATE Boost::log backtrace)
