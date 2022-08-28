add_subdirectory(glad)
add_subdirectory(glm)

add_library(stb INTERFACE stb/stb_image.h)
target_include_directories(stb PUBLIC INTERFACE stb)
