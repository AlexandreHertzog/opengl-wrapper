add_subdirectory(opengl-cpp)

add_library(stb INTERFACE stb/stb_image.h)
target_include_directories(stb PUBLIC INTERFACE stb)

add_library(imgui_backend imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_glfw.cpp)
target_include_directories(imgui_backend PUBLIC imgui imgui/backends)

add_library(imgui imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp)
target_link_libraries(imgui PUBLIC imgui_backend)
