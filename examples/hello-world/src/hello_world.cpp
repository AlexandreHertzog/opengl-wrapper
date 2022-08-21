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
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

        bool wireframe = true;
        window.set_key_action(GLFW_KEY_BACKSPACE, [&](int action) {
            if (GLFW_PRESS == action) {
                window.set_wireframe_mode(wireframe);
                wireframe = !wireframe;
            }
        });

        auto square_program = std::make_shared<opengl_wrapper::program>();

        square_program->add_shader(
            opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/square.vert")));

        square_program->add_shader(
            opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/square.frag")));

        square_program->link();

        opengl_wrapper::shape square;
        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 0.0F}});

        square.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});

        square.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});

        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});

        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});

        square.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        square.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        square.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        square.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});

        square.set_program(square_program);

        auto container_texture = window.get_renderer().add_texture("textures/container.jpg", GL_TEXTURE0);
        auto awesomeface_texture = window.get_renderer().add_texture("textures/awesomeface.png", GL_TEXTURE1);

        square.get_program()->use();
        square.get_program()->set_uniform("texture1", 0);
        square.get_program()->set_uniform("texture2", 1);

        std::vector<glm::vec3> cube_positions = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                                 glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                                 glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                                 glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                                 glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        auto cube_index = 0;
        square.get_program()->set_use_callback([&](opengl_wrapper::program &p) {
            auto model = glm::mat4(1.0F);
            model = glm::translate(model, cube_positions.at(cube_index));
            model = glm::rotate(model, glm::radians(20.0F * cube_index), glm::vec3(1.0F, 0.3F, 0.5F));

            auto view = glm::mat4(1.0F);
            view = glm::translate(view, glm::vec3(0.0F, 0.0F, -3.0F));

            auto projection = glm::perspective(glm::radians(45.0F), 800.0F / 600.0F, 0.1F, 100.0F);

            p.set_uniform("model", glm::value_ptr(model));
            p.set_uniform("view", glm::value_ptr(view));
            p.set_uniform("projection", glm::value_ptr(projection));

            if (cube_index++ >= cube_positions.size() - 1) {
                cube_index = 0;
            }
        });

        square.set_textures({std::move(container_texture), std::move(awesomeface_texture)});

        for (auto i = 0; i < cube_positions.size(); i++) {
            window.get_renderer().add_shape(square);
        }

        window.render_loop();
        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop