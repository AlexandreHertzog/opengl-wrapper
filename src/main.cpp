#include "integration.h"
#include <iostream>

int main() {
    try {
        game_engine::integration_t app;
        app.init_callbacks();

        app.build_shapes();
        app.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << "Forced termination with error: " << e.what();
        return 1;
    }
}
