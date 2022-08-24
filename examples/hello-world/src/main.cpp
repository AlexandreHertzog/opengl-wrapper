#include "integration.h"
#include <iostream>

int main() {
    try {
        test_app::integration app;
        app.init_callbacks();

        auto cube = app.build_cube();
        cube.set_program(app.build_program());
        app.init_textures(cube);
        app.init_shapes(cube);
        app.prepare_render_loop();
        app.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}
