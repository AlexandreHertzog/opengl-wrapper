#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window_manager.h"

#include <iostream>

int main(int argc, char **argv) {
    try {
        auto &gl_manager = opengl_wrapper::GlManager::instance();
        auto &window_manager = opengl_wrapper::WindowManager::instance();
        auto window = window_manager.createWindow(800, 600, "Hello world!");

        window->setKeyAction(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                glfwSetWindowShouldClose(window->getWindow(), true);
            }
        });

        window->renderLoop();
        return 0;
    } catch (std::runtime_error e) {
        std::cerr << e.what();
        return 1;
    }
}
