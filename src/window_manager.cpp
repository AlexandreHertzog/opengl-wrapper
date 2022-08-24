#include "window_manager.h"

#include "data_types/shape.h"
#include "data_types/window.h"
#include "graphics/graphics.h"
#include "utils/glad_error.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

namespace opengl_wrapper {

std::map<const void *, window_manager *> window_manager::m_windows_map;

window_manager::window_manager()
    : m_resize_handler([](GLFWwindow *window, int width, int height) {
          auto window_manager = m_windows_map.find(window);
          if (m_windows_map.end() == window_manager) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          graphics::instance().gl_viewport(0, 0, width, height);
          window_manager->second->m_window->draw(window_manager->second->m_shapes);
      }),
      m_key_handler([](GLFWwindow *window, int key, int scancode, int action, int mods) {
          auto window_manager = m_windows_map.find(window);
          if (m_windows_map.end() == window_manager) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          const auto existing_action = window_manager->second->m_action_map.find(key);
          if (window_manager->second->m_action_map.end() != existing_action) {
              existing_action->second(action);
          }
      }),
      m_cursor_pos_handler([](GLFWwindow *window, double xpos, double ypos) {
          auto window_manager = m_windows_map.find(window);
          if (m_windows_map.end() == window_manager) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          if (window_manager->second->m_app_cursor_pos_callback) {
              (*window_manager->second->m_app_cursor_pos_callback)(xpos, ypos);
          }
      }),
      m_initialized(false), m_frame_time_us(0.0), m_camera({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}) {

    set_refresh_rate(60); // NOLINT(*-magic-numbers)
}

void window_manager::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug) << "window_manager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ")";

    assert(!m_initialized);

    m_window = std::make_unique<window>(width, height, title);
    m_windows_map[m_window->get_window()] = this;

    m_window->set_as_context();
    m_window->set_framebuffer_callback(m_resize_handler);
    m_window->set_key_callback(m_key_handler);
    m_window->set_cursor_pos_callback(m_cursor_pos_handler);

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        BOOST_LOG_TRIVIAL(trace) << "window_manager::init(width=" << width << ", height=" << height
                                 << ", title=" << title << ") glad_error";
        throw glad_error("gladLoadGLLoader() failed");
    }

    m_initialized = true;

    int num_vertex_attributes = 0;
    graphics::instance().gl_get_integerv(GL_MAX_VERTEX_ATTRIBS, &num_vertex_attributes);
    BOOST_LOG_TRIVIAL(info) << "GL_MAX_VERTEX_ATTRIBS = " << num_vertex_attributes;
}

void window_manager::render_loop() noexcept {
    assert(m_initialized);

    load_vertices();

    graphics::instance().gl_enable(GL_DEPTH_TEST);

    while (1 != m_window->get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        graphics::instance().gl_clear_color(0.2F, 0.2F, 0.2F, 1.0F);
        graphics::instance().gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_window->draw(m_shapes);

        m_window->swap_buffers();
        graphics::instance().glfw_poll_events();

        std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

        if (loop_time_us.count() < m_frame_time_us) {
            const auto wait_time_us = m_frame_time_us - loop_time_us.count();
            BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_us));
        } else {
            BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
        }
    }
}

void window_manager::set_key_action(int key, key_callback_t action) noexcept {
    auto existing_action = m_action_map.find(key);
    if (m_action_map.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        m_action_map[key] = std::move(action);
    }
}

void window_manager::set_cursor_position_callback(window_manager::cursor_pos_callback_t callback) {
    m_app_cursor_pos_callback = std::make_unique<cursor_pos_callback_t>(std::move(callback));
}

void window_manager::set_window_should_close(int value) noexcept {
    assert(m_initialized);
    m_window->set_should_close(value);
}

void window_manager::set_cursor_enabled(bool enabled) {
    m_window->set_input_mode(GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void window_manager::set_refresh_rate(int refresh_rate) noexcept {
    constexpr auto s_to_us_multiplier = 1000000.0;
    assert(refresh_rate > 0);
    m_frame_time_us = s_to_us_multiplier / refresh_rate;
}

void window_manager::set_wireframe_mode(bool wireframe) {
    if (wireframe) {
        graphics::instance().gl_polygon_mode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        graphics::instance().gl_polygon_mode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

camera &window_manager::get_camera() {
    return m_camera;
}

void window_manager::add_shape(shape s) {
    m_shapes.emplace_back(std::move(s));
}

void window_manager::load_vertices() {
    for (auto &shape : m_shapes) {
        shape.get_vertex_array().load(shape.get_vertices(), shape.get_draw_order(), GL_STATIC_DRAW);
    }
}

std::shared_ptr<texture> window_manager::add_texture(const std::filesystem::path &path, int unit) {
    return m_textures.add_texture(path, unit);
}

} // namespace opengl_wrapper