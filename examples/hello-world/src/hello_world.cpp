#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp" // No need to validate allocation problems here, it is a test program.

#include "opengl-wrapper/gl_manager.h"
#include "opengl-wrapper/models/shape.h"
#include "opengl-wrapper/renderer.h"
#include "opengl-wrapper/window_manager.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/shader.h"
#include <boost/log/trivial.hpp>
#include <complex>
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

        auto first_program = std::make_shared<opengl_wrapper::program>();

        first_program->add_shader(
            opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/shader.vert")));

        first_program->add_shader(
            opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/triangle1.frag")));

        first_program->link();

        first_program->set_use_callback([](opengl_wrapper::program &program) {
            auto time_value = glfwGetTime();
            auto green_value = sin(time_value) / 2.0F + 0.5F; // NOLINT(*-magic-numbers)
            int runtime_color = program.get_uniform_location("runtime_color");
            glUniform4f(runtime_color, 0.0F, static_cast<float>(green_value), 0.0F, 1.0F);
        });

        auto second_program = std::make_shared<opengl_wrapper::program>();

        second_program->add_shader(
            opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/shader.vert")));

        second_program->add_shader(
            opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/triangle2.frag")));

        second_program->link();

        opengl_wrapper::shape triangle1;
        std::vector<unsigned> triangle1_vert_order(3);
        triangle1_vert_order[0] = triangle1.add_vertex({{-0.9F, -0.5F, 0.0F}, {1.0F, 1.0F, 1.0F}});
        triangle1_vert_order[1] = triangle1.add_vertex({{-0.0F, -0.5F, 0.0F}, {1.0F, 1.0F, 1.0F}});
        triangle1_vert_order[2] = triangle1.add_vertex({{-0.45F, 0.5F, 0.0F}, {1.0F, 1.0F, 1.0F}});
        triangle1.set_draw_order(std::move(triangle1_vert_order));
        triangle1.set_program(first_program);

        opengl_wrapper::shape triangle2;
        std::vector<unsigned> triangle2_vert_order(3);
        triangle2_vert_order[0] = triangle2.add_vertex({{0.0F, -0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}});
        triangle2_vert_order[1] = triangle2.add_vertex({{0.9F, -0.5F, 0.0F}, {0.0F, 0.1F, 0.0F}});
        triangle2_vert_order[2] = triangle2.add_vertex({{0.45F, 0.5F, 0.0F}, {0.0F, 0.0F, 1.0F}});
        triangle2.set_draw_order(std::move(triangle2_vert_order));
        triangle2.set_program(second_program);

        const std::vector<unsigned int> indices = {0, 1, 2};

        window.get_renderer().add_shape(std::move(triangle1));
        window.get_renderer().add_shape(std::move(triangle2));
        window.get_renderer().load_vertices();

        window.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop