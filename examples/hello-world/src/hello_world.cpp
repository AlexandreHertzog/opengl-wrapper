#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window.h"

#include <iostream>

int main(int argc, char **argv) {
    try {
        auto &gl_manager = opengl_wrapper::GlManager::instance();
        auto &window = opengl_wrapper::Window::instance();
        window.init(800, 600, "Hello world!");

        window.setKeyAction(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.setWindowShouldClose(true);
            }
        });

        window.renderLoop();
        return 0;
    } catch (std::runtime_error e) {
        std::cerr << e.what();
        return 1;
    }
}
