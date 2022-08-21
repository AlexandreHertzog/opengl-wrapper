add_executable(hello-world src/hello_world.cpp)
target_link_libraries(hello-world PRIVATE opengl-wrapper Boost::log glm)