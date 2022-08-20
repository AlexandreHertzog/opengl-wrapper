#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp" // No need to validate allocation problems here, it is a test program.

#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/models/shape.h"
#include "opengl-wrapper/renderer.h"
#include "opengl-wrapper/textures/texture_controller.h"
#include "opengl-wrapper/window_manager.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/shader.h"
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <iostream>

int main() {
    try {
        auto &gl_manager = opengl_wrapper::gl_manager::instance();
        auto &window = opengl_wrapper::window_manager::instance();
        window.init(800, 600, "Hello world!"); // NOLINT(*-magic-numbers)

        window.set_key_action(GLFW_KEY_ESCAPE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.set_window_should_close(1);
            }
        });

        auto square_program = std::make_shared<opengl_wrapper::program>();

        square_program->add_shader(
            opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/square.vert")));

        square_program->add_shader(
            opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/square.frag")));

        square_program->link();

        opengl_wrapper::shape square;
        square.add_vertex({{0.5F, 0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, 0.0F}, {0.0F, 1.0F, 0.0F}, {1.0F, 0.0F}});
        square.add_vertex({{-0.5F, -0.5F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}});
        square.add_vertex({{-0.5F, 0.5F, 0.0F}, {1.0F, 1.0F, 1.0F}, {0.0F, 1.0F}});
        square.set_draw_order({0, 1, 3, 1, 2, 3});
        square.set_program(square_program);

        square.set_texture(window.get_renderer().add_texture("textures/container.jpg"));

        window.get_renderer().add_shape(std::move(square));
        window.get_renderer().load_vertices();

        window.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop