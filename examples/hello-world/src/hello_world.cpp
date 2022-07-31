#include "opengl-wrapper/gl_manager.h"

#include <iostream>

int main(int argc, char **argv) {
    try {
        auto gl_manager = opengl_wrapper::gl_manager::instance();
        auto window = gl_manager.display_window(800, 600, "Hello world!");
        gl_manager.render_loop(window);
        return 0;
    } catch (std::runtime_error e) {
        std::cerr << e.what();
        return 1;
    }
}
