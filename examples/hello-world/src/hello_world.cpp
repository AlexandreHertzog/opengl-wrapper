#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/window.h"

#include <iostream>

int main(int argc, char **argv) {
    try {
        auto gl_manager = opengl_wrapper::GlManager::instance();
        auto window = opengl_wrapper::Window(800, 600, "Hello world!");
        window.renderLoop();
        return 0;
    } catch (std::runtime_error e) {
        std::cerr << e.what();
        return 1;
    }
}
