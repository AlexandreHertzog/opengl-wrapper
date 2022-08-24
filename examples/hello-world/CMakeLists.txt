add_executable(hello-world src/main.cpp src/integration.cpp src/integration.h)
target_link_libraries(hello-world PRIVATE opengl-wrapper Boost::log glm)