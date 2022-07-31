#include "opengl-wrapper/main_manager.h"

#include <iostream>

int main(int argc, char **argv) {
    try {
        auto main_manager = opengl_wrapper::main_manager::instance();
        auto window = main_manager.display_window(800, 600, "Hello world!");
        main_manager.render_loop(window);
        return 0;
    } catch (std::runtime_error e) {
        std::cerr << e.what();
        return 1;
    }
}
