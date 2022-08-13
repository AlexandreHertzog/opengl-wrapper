#include "window_manager.h"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

#include "exceptions/glad_error.h"
#include "wraps/buffer.h"

namespace opengl_wrapper {

WindowManager::WindowManager()
    : resize_handler_([](GLFWwindow *window, int width, int height) {
          BOOST_LOG_TRIVIAL(debug) << "WindowManager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ")";

          auto &window_manager = WindowManager::instance();
          if (*window_manager.window_ != window) {
              BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << window;
              return;
          }

          glViewport(0, 0, width, height);

          WindowManager::instance().getRenderer().draw();

          BOOST_LOG_TRIVIAL(trace) << "WindowManager::resize_handler_(window=" << window << ", width=" << width
                                   << ", height=" << height << ") end";
      }),
      key_handler_([](GLFWwindow *window, int key, int scancode, int action, int mods) {
          BOOST_LOG_TRIVIAL(debug) << "window_manager::key_handler_(window=" << window << ", key=" << key
                                   << ", scancode=" << scancode << ", mods=" << mods << ")";

          auto &window_manager = WindowManager::instance();
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

    BOOST_LOG_TRIVIAL(debug) << "WindowManager::WindowManager()";
    setRefreshRate(60); // NOLINT(*-magic-numbers)
    BOOST_LOG_TRIVIAL(trace) << "WindowManager::WindowManager() end";
}

WindowManager &WindowManager::instance() {
    static WindowManager static_instance;
    return static_instance;
}

Renderer &WindowManager::getRenderer() {
    assert(renderer_);
    return *renderer_;
}

void WindowManager::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug) << "WindowManager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ")";

    assert(!initialized_);

    window_ = std::make_unique<Window>(width, height, title);
    window_->makeCurrentContext();

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        BOOST_LOG_TRIVIAL(trace) << "WindowManager::init(width=" << width << ", height=" << height
                                 << ", title=" << title << ") GladError";
        throw GladError("gladLoadGLLoader() failed");
    }

    window_->setFramebufferSizeCallback(resize_handler_);
    window_->setKeyCallback(key_handler_);

    glViewport(0, 0, width, height);

    renderer_ = std::make_unique<Renderer>();

    initialized_ = true;

    BOOST_LOG_TRIVIAL(trace) << "WindowManager::init(width=" << width << ", height=" << height << ", title=" << title
                             << ") end";
}

void WindowManager::renderLoop() noexcept {
    BOOST_LOG_TRIVIAL(debug) << "WindowManager::renderLoop()";

    assert(initialized_);

    while (1 != window_->shouldClose()) {
        auto start_time = std::chrono::high_resolution_clock::now();

        window_->swapBuffers();

        glfwPollEvents();

        std::chrono::duration<double, std::micro> loop_time_us = std::chrono::high_resolution_clock::now() - start_time;

        BOOST_LOG_TRIVIAL(debug) << "renderLoop() frame_time_us_=" << frame_time_us_
                                 << ", loop_time_us=" << loop_time_us.count();

        if (loop_time_us.count() < frame_time_us_) {
            const auto wait_time_us = frame_time_us_ - loop_time_us.count();
            BOOST_LOG_TRIVIAL(debug) << "Waiting " << wait_time_us << "ms to fill frame_time";
            usleep(static_cast<unsigned int>(wait_time_us));
        } else {
            BOOST_LOG_TRIVIAL(debug) << "loop_time too large, skipping time filler";
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "WindowManager::renderLoop() end";
}

void WindowManager::setKeyAction(int key, Action action) noexcept {
    auto existing_action = action_map_.find(key);
    if (action_map_.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        action_map_[key] = std::move(action);
    }
}

void WindowManager::setWindowShouldClose(int value) noexcept {
    assert(initialized_);
    window_->setShouldClose(value);
}

void WindowManager::setRefreshRate(int refresh_rate) noexcept {
    constexpr auto s_to_us_multiplier = 1000000.0;
    assert(refresh_rate > 0);
    frame_time_us_ = s_to_us_multiplier / refresh_rate;
}

} // namespace opengl_wrapper