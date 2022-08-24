#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp" // No need to validate allocation problems here, it is a test program.

#include "opengl-wrapper/data_types/camera.h"
#include "opengl-wrapper/data_types/program.h"
#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/data_types/window.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/texture_controller.h"
#include <boost/log/trivial.hpp>
#include <csignal>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {
    try {
        auto window = opengl_wrapper::window(800, 600, "Hello world!"); // NOLINT(*-magic-numbers)

        std::vector<opengl_wrapper::shape> shapes;

        window.set_framebuffer_callback([&shapes](opengl_wrapper::window &w, int width, int height) {
            w.set_viewport(width, height);
            w.draw(shapes);
        });

        bool wireframe = false;
        bool cursor_enabled = false;

        constexpr auto camera_speed = 0.1F;
        opengl_wrapper::camera camera({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0});
        camera.set_position(glm::vec3(0.0F, 0.0F, 3.0F));
        camera.set_up(glm::vec3(0.0, 1.0, 0.0));

        window.set_key_callback([&wireframe, &cursor_enabled, &camera](opengl_wrapper::window &w, int key, int scancode,
                                                                       int action, int mods) {
            if (GLFW_PRESS == action && GLFW_KEY_ESCAPE == key) {
                w.set_should_close(1);
            }
            if (GLFW_PRESS == action && GLFW_KEY_BACKSPACE == key) {
                wireframe = !wireframe;
                w.set_wireframe_mode(wireframe);
            }
            if (GLFW_PRESS == action && GLFW_KEY_SPACE == key) {
                cursor_enabled = !cursor_enabled;
                w.set_input_mode(GLFW_CURSOR, cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            }
            if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_W == key) {
                camera.set_position(camera.get_position() + camera_speed * camera.get_front());
            }
            if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_S == key) {
                camera.set_position(camera.get_position() - camera_speed * camera.get_front());
            }
            if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_A == key) {
                camera.set_position(camera.get_position() -
                                    camera_speed * glm::normalize(glm::cross(camera.get_front(), camera.get_up())));
            }
            if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_D == key) {
                camera.set_position(camera.get_position() +
                                    camera_speed * glm::normalize(glm::cross(camera.get_front(), camera.get_up())));
            }
        });

        bool first_cursor_iteration = true;
        double last_cursor_xpos = 0.0;
        double last_cursor_ypos = 0.0;
        auto pitch = 0.0;
        auto yaw = -90.0;
        window.set_cursor_pos_callback([&](opengl_wrapper::window &w, double xpos, double ypos) {
            if (first_cursor_iteration) {
                last_cursor_xpos = xpos;
                last_cursor_ypos = ypos;
                first_cursor_iteration = false;
            }

            constexpr auto sensitivity = 0.1;
            const auto xpos_offset = (xpos - last_cursor_xpos) * sensitivity;
            const auto ypos_offset = (last_cursor_ypos - ypos) * sensitivity;

            last_cursor_xpos = xpos;
            last_cursor_ypos = ypos;

            yaw += xpos_offset;
            pitch = std::max(std::min(pitch + ypos_offset, 89.0), -89.0);

            camera.set_front(pitch, yaw);
        });

        auto square_program = std::make_shared<opengl_wrapper::program>();

        square_program->add_shader(
            opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/square.vert")));

        square_program->add_shader(
            opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/square.frag")));

        square_program->link();

        opengl_wrapper::shape cube;
        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 0.0F}});

        cube.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});

        cube.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});

        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});

        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{0.5F, -0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{0.5F, -0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{-0.5F, -0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{-0.5F, -0.5F, -0.5F}, {}, {0.0F, 1.0F}});

        cube.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});
        cube.add_vertex({{0.5F, 0.5F, -0.5F}, {}, {1.0F, 1.0F}});
        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{0.5F, 0.5F, 0.5F}, {}, {1.0F, 0.0F}});
        cube.add_vertex({{-0.5F, 0.5F, 0.5F}, {}, {0.0F, 0.0F}});
        cube.add_vertex({{-0.5F, 0.5F, -0.5F}, {}, {0.0F, 1.0F}});

        cube.set_program(square_program);

        opengl_wrapper::texture_controller tex_controller;

        auto container_texture = tex_controller.add_texture("textures/container.jpg", GL_TEXTURE0);
        auto awesomeface_texture = tex_controller.add_texture("textures/awesomeface.png", GL_TEXTURE1);

        cube.get_program()->use();
        cube.get_program()->set_uniform("texture1", 0);
        cube.get_program()->set_uniform("texture2", 1);

        std::vector<glm::vec3> cube_positions = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                                 glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                                 glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                                 glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                                 glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        auto cube_index = 0;
        cube.get_program()->set_use_callback([&](opengl_wrapper::program &p) {
            auto model = glm::mat4(1.0F);
            model = glm::translate(model, cube_positions.at(cube_index));
            model = glm::rotate(model, glm::radians(20.0F * cube_index), glm::vec3(1.0F, 0.3F, 0.5F));

            auto view = camera.look_at(camera.get_position() + camera.get_front());

            auto projection = glm::perspective(glm::radians(45.0F), 800.0F / 600.0F, 0.1F, 100.0F);

            p.set_uniform("model", glm::value_ptr(model));
            p.set_uniform("view", glm::value_ptr(view));
            p.set_uniform("projection", glm::value_ptr(projection));

            if (cube_index++ >= cube_positions.size() - 1) {
                cube_index = 0;
            }
        });

        cube.set_textures({std::move(container_texture), std::move(awesomeface_texture)});

        for (auto i = 0; i < cube_positions.size(); i++) {
            shapes.emplace_back(cube);
        }

        window.set_depth_test(true);
        window.set_clear_color(0.2F, 0.2F, 0.2F, 1.0F);

        constexpr auto s_to_us_multiplier = 1000000.0;
        auto frame_time_us = s_to_us_multiplier / 60;

        for (auto &shape : shapes) {
            shape.get_vertex_array().load(shape.get_vertices(), shape.get_draw_order(), GL_STATIC_DRAW);
        }

        while (!window.get_should_close()) {
            auto start_time = std::chrono::high_resolution_clock::now();
            window.draw(shapes);

            std::chrono::duration<double, std::micro> loop_time_us =
                std::chrono::high_resolution_clock::now() - start_time;

            if (loop_time_us.count() < frame_time_us) {
                const auto wait_time_us = frame_time_us - loop_time_us.count();
                BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
                usleep(static_cast<unsigned int>(wait_time_us));
            } else {
                BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
            }
        }

        return 0;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
}

#pragma clang diagnostic pop