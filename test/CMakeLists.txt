include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(graphics INTERFACE src/mock_graphics.h)
target_link_libraries(graphics PUBLIC INTERFACE opengl-wrapper gmock)

add_executable(autotest src/test_buffer.cpp src/test_shader.cpp src/test_texture.cpp)
target_link_libraries(autotest PRIVATE graphics gtest_main)
