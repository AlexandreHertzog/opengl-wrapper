#include "window_manager.h"

#include "models/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/glad_error.h"
#include "renderer.h"
#include "wraps/window.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

namespace opengl_wrapper {

window_manager::window_manager()
    : m_resize_handler([](GLFWwindow *window, int width, int height) {
          BOOST_LOG_TRIVIAL(debug) << "window_manager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ")";

          auto &window_manager = window_manager::instance();
          if (*window_manager.m_window != window) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          graphics::instance().gl_viewport(0, 0, width, height);

          window_manager::instance().get_renderer().draw();

          BOOST_LOG_TRIVIAL(trace) << "window_manager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ") end";
      }),
      m_key_handler([](GLFWwindow *window, int key, int scancode, int action, int mods) {
          BOOST_LOG_TRIVIAL(debug) << "window_manager::key_handler_(window=" << window << ", key=" << key
                                   << ", scancode=" << scancode << ", mods=" << mods << ")";

          auto &window_manager = window_manager::instance();
          if (*window_manager.m_window != window) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          const auto existing_action = window_manager.m_action_map.find(key);
          if (window_manager.m_action_map.end() != existing_action) {
              existing_action->second(action);
          }

          BOOST_LOG_TRIVIAL(trace) << "window_manager::key_handler_(window=" << window << ", key=" << key
                                   << ", scancode=" << scancode << ", mods=" << mods << ") end";
      }),
      m_initialized(false), m_frame_time_us(0.0) {

    BOOST_LOG_TRIVIAL(debug) << "window_manager::window_manager()";
    set_refresh_rate(60); // NOLINT(*-magic-numbers)
    BOOST_LOG_TRIVIAL(trace) << "window_manager::window_manager() end";
}

window_manager &window_manager::instance() {
    static window_manager static_instance;
    return static_instance;
}

renderer &window_manager::get_renderer() {
    assert(m_renderer);
    return *m_renderer;
}

void window_manager::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug) << "window_manager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ")";

    assert(!m_initialized);

    m_window = std::make_unique<window>(width, height, title);
    m_window->set_as_context();
    m_window->set_framebuffer_callback(m_resize_handler);
    m_window->set_key_callback(m_key_handler);

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        BOOST_LOG_TRIVIAL(trace) << "window_manager::init(width=" << width << ", height=" << height
                                 << ", title=" << title << ") glad_error";
        throw glad_error("gladLoadGLLoader() failed");
    }

    m_renderer = std::make_unique<renderer>();

    m_initialized = true;

    int num_vertex_attributes = 0;
    graphics::instance().gl_get_integerv(GL_MAX_VERTEX_ATTRIBS, &num_vertex_attributes);
    BOOST_LOG_TRIVIAL(info) << "GL_MAX_VERTEX_ATTRIBS = " << num_vertex_attributes;

    BOOST_LOG_TRIVIAL(trace) << "window_manager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ") end";
}

void window_manager::render_loop() noexcept {
    BOOST_LOG_TRIVIAL(debug) << "window_manager::render_loop()";

    assert(m_initialized);

    m_renderer->load_vertices();

    graphics::instance().gl_enable(GL_DEPTH_TEST);

    while (1 != m_window->get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        graphics::instance().gl_clear_color(0.2F, 0.2F, 0.2F, 1.0F);
        graphics::instance().gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_renderer->draw();

        m_window->swap_buffers();
        graphics::instance().glfw_poll_events();

        std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

        BOOST_LOG_TRIVIAL(debug) << "render_loop() frame_time_us_=" << m_frame_time_us
                                 << ", loop_time_us=" << loop_time_us.count();

        if (loop_time_us.count() < m_frame_time_us) {
            const auto wait_time_us = m_frame_time_us - loop_time_us.count();
            BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_us));
        } else {
            BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "window_manager::render_loop() end";
}

void window_manager::set_key_action(int key, Action action) noexcept {
    auto existing_action = m_action_map.find(key);
    if (m_action_map.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        m_action_map[key] = std::move(action);
    }
}

void window_manager::set_window_should_close(int value) noexcept {
    assert(m_initialized);
    m_window->set_should_close(value);
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

} // namespace opengl_wrapper