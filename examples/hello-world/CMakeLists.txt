add_executable(hello-world src/main.cpp)
target_link_libraries(hello-world PRIVATE opengl-wrapper Boost::log glm)