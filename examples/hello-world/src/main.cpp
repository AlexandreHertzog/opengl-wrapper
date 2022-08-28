#include "integration.h"
#include <iostream>

int main() {
    try {
        test_app::integration app;
        app.init_callbacks();

        app.build_shapes();
        app.prepare_render_loop();
        app.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}
