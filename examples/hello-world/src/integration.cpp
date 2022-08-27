#include "integration.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <boost/log/trivial.hpp>
#include <csignal>

namespace test_app {

integration::integration()
    : m_window(800, 600, "Test application"), m_camera({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}) {

    m_camera.set_position(glm::vec3(0.0F, 0.0F, 3.0F));
    m_camera.set_up(glm::vec3(0.0, 1.0, 0.0));
}

void integration::init_callbacks() {
    constexpr auto camera_speed = 0.1F;

    m_window.set_framebuffer_callback([&](opengl_wrapper::window &w, int width, int height) {
        w.set_viewport(width, height);
        w.draw(m_shapes);
    });

    //    m_window.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_window.set_key_callback([&](opengl_wrapper::window &w, int key, int scancode, int action, int mods) {
        if (GLFW_PRESS == action && GLFW_KEY_ESCAPE == key) {
            w.set_should_close(1);
        }
        if (GLFW_PRESS == action && GLFW_KEY_BACKSPACE == key) {
            m_wireframe = !m_wireframe;
            w.set_wireframe_mode(m_wireframe);
        }
        if (GLFW_PRESS == action && GLFW_KEY_SPACE == key) {
            m_cursor_enabled = !m_cursor_enabled;
            w.set_input_mode(GLFW_CURSOR, m_cursor_enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_W == key) {
            m_camera.set_position(m_camera.get_position() + camera_speed * m_camera.get_front());
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_S == key) {
            m_camera.set_position(m_camera.get_position() - camera_speed * m_camera.get_front());
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_A == key) {
            m_camera.set_position(m_camera.get_position() -
                                  camera_speed * glm::normalize(glm::cross(m_camera.get_front(), m_camera.get_up())));
        }
        if (((GLFW_PRESS == action) || (GLFW_REPEAT == action)) && GLFW_KEY_D == key) {
            m_camera.set_position(m_camera.get_position() +
                                  camera_speed * glm::normalize(glm::cross(m_camera.get_front(), m_camera.get_up())));
        }
    });

    m_window.set_cursor_pos_callback([&](opengl_wrapper::window &w, double xpos, double ypos) {
        if (m_first_cursor_iteration) {
            m_last_cursor_xpos = xpos;
            m_last_cursor_ypos = ypos;
            m_first_cursor_iteration = false;
        }

        constexpr auto sensitivity = 0.1;
        const auto xpos_offset = (xpos - m_last_cursor_xpos) * sensitivity;
        const auto ypos_offset = (m_last_cursor_ypos - ypos) * sensitivity;

        m_last_cursor_xpos = xpos;
        m_last_cursor_ypos = ypos;

        m_yaw += xpos_offset;
        m_pitch = std::max(std::min(m_pitch + ypos_offset, 89.0), -89.0);

        m_camera.set_front(m_pitch, m_yaw);
    });
}

std::shared_ptr<opengl_wrapper::program> integration::build_program() {
    auto program = std::make_shared<opengl_wrapper::program>();
    program->add_shader(opengl_wrapper::shader(GL_VERTEX_SHADER, std::filesystem::path("shaders/square.vert")));
    program->add_shader(opengl_wrapper::shader(GL_FRAGMENT_SHADER, std::filesystem::path("shaders/square.frag")));
    program->link();

    program->set_use_callback([&](opengl_wrapper::program &p, opengl_wrapper::shape &s) {
        auto model = glm::mat4(1.0F);
        model = glm::translate(model, s.get_translation());
        model = glm::rotate(model, glm::radians(s.get_rotation_angle()), s.get_rotation_axis());
        model = glm::scale(model, s.get_scale());

        auto view = m_camera.look_at(m_camera.get_position() + m_camera.get_front());
        auto projection = glm::perspective(glm::radians(45.0F), 800.0F / 600.0F, 0.1F, 100.0F);

        p.set_uniform("model", glm::value_ptr(model));
        p.set_uniform("view", glm::value_ptr(view));
        p.set_uniform("projection", glm::value_ptr(projection));
    });
    return program;
}

void integration::build_shapes() {
    auto cube = opengl_wrapper::shape::build_from_file("./objects/cube.obj");
    cube.set_rotation(45.0F, glm::vec3(1.0, 0.5, 0.5));
    cube.set_scale(glm::vec3(1.0, 0.5, 0.5));
    cube.set_program(build_program());
    m_shapes.emplace_back(std::move(cube));
}

void integration::init_textures() {
    auto container_texture = std::make_shared<opengl_wrapper::texture>(GL_TEXTURE0);
    container_texture->set_image_from_path("textures/container.jpg");

    auto awesomeface_texture = std::make_shared<opengl_wrapper::texture>(GL_TEXTURE1);
    awesomeface_texture->set_image_from_path("textures/awesomeface.png");

    for (auto &s : m_shapes) {
        s.set_textures({container_texture, awesomeface_texture});
        s.get_program()->use(s);
        s.get_program()->set_uniform("texture1", 0);
        s.get_program()->set_uniform("texture2", 1);
    }
}

void integration::prepare_render_loop() {
    m_window.set_depth_test(true);
    m_window.set_clear_color(0.2F, 0.2F, 0.2F, 1.0F);

    for (auto &shape : m_shapes) {
        shape.get_vertex_array().load(shape.serialize_vertices(), shape.serialize_draw_order(), GL_STATIC_DRAW);
    }
}

void integration::render_loop() {
    constexpr auto s_to_us_multiplier = 1000000.0;
    auto frame_time_us = s_to_us_multiplier / 60;

    while (!m_window.get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();
        m_window.draw(m_shapes);

        std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

        if (loop_time_us.count() < frame_time_us) {
            const auto wait_time_us = frame_time_us - loop_time_us.count();
            BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_us));
        } else {
            BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
        }
    }
}

} // namespace test_app