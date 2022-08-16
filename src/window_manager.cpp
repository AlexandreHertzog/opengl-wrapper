#include "window_manager.h"

#include "exceptions/glad_error.h"
#include "models/shape.h"
#include "renderer.h"
#include "wraps/window.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

namespace opengl_wrapper {

window_manager::window_manager()
    : resize_handler_([](GLFWwindow *window, int width, int height) {
          BOOST_LOG_TRIVIAL(debug) << "window_manager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ")";

          auto &window_manager = window_manager::instance();
          if (*window_manager.window_ != window) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          glViewport(0, 0, width, height);

          window_manager::instance().get_renderer().draw();

          BOOST_LOG_TRIVIAL(trace) << "window_manager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ") end";
      }),
      key_handler_([](GLFWwindow *window, int key, int scancode, int action, int mods) {
          BOOST_LOG_TRIVIAL(debug) << "window_manager::key_handler_(window=" << window << ", key=" << key
                                   << ", scancode=" << scancode << ", mods=" << mods << ")";

          auto &window_manager = window_manager::instance();
          if (*window_manager.window_ != window) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          const auto existing_action = window_manager.action_map_.find(key);
          if (window_manager.action_map_.end() != existing_action) {
              existing_action->second(action);
          }

          BOOST_LOG_TRIVIAL(trace) << "window_manager::key_handler_(window=" << window << ", key=" << key
                                   << ", scancode=" << scancode << ", mods=" << mods << ") end";
      }),
      initialized_(false), frame_time_us_(0.0) {

    BOOST_LOG_TRIVIAL(debug) << "window_manager::window_manager()";
    set_refresh_rate(60); // NOLINT(*-magic-numbers)
    BOOST_LOG_TRIVIAL(trace) << "window_manager::window_manager() end";
}

window_manager &window_manager::instance() {
    static window_manager static_instance;
    return static_instance;
}

renderer &window_manager::get_renderer() {
    assert(renderer_);
    return *renderer_;
}

void window_manager::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug) << "window_manager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ")";

    assert(!initialized_);

    window_ = std::make_unique<window>(width, height, title);
    window_->set_as_context();

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        BOOST_LOG_TRIVIAL(trace) << "window_manager::init(width=" << width << ", height=" << height
                                 << ", title=" << title << ") glad_error";
        throw glad_error("gladLoadGLLoader() failed");
    }

    window_->set_framebuffer_callback(resize_handler_);
    window_->set_key_callback(key_handler_);

    glViewport(0, 0, width, height);

    renderer_ = std::make_unique<renderer>();

    initialized_ = true;

    int num_vertex_attributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_vertex_attributes);
    BOOST_LOG_TRIVIAL(info) << "GL_MAX_VERTEX_ATTRIBS = " << num_vertex_attributes;

    BOOST_LOG_TRIVIAL(trace) << "window_manager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ") end";
}

void window_manager::render_loop() noexcept {
    BOOST_LOG_TRIVIAL(debug) << "window_manager::render_loop()";

    assert(initialized_);

    while (1 != window_->get_should_close()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer_->draw();

        window_->swap_buffers();
        glfwPollEvents();

        std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

        BOOST_LOG_TRIVIAL(debug) << "render_loop() frame_time_us_=" << frame_time_us_
                                 << ", loop_time_us=" << loop_time_us.count();

        if (loop_time_us.count() < frame_time_us_) {
            const auto wait_time_us = frame_time_us_ - loop_time_us.count();
            BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_us));
        } else {
            BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "window_manager::render_loop() end";
}

void window_manager::set_key_action(int key, Action action) noexcept {
    auto existing_action = action_map_.find(key);
    if (action_map_.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        action_map_[key] = std::move(action);
    }
}

void window_manager::set_window_should_close(int value) noexcept {
    assert(initialized_);
    window_->set_should_close(value);
}

void window_manager::set_refresh_rate(int refresh_rate) noexcept {
    constexpr auto s_to_us_multiplier = 1000000.0;
    assert(refresh_rate > 0);
    frame_time_us_ = s_to_us_multiplier / refresh_rate;
}

} // namespace opengl_wrapper