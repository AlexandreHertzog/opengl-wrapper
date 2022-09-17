add_library(game-engine-utils exception.cpp)
target_link_libraries(game-engine-utils PRIVATE Boost::log backtrace)
