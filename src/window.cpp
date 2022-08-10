#include "window.h"

#include <boost/log/trivial.hpp>
#include <cassert>
#include <chrono>
#include <unistd.h>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"
#include "wraps/buffer.h"

namespace opengl_wrapper {

Window::Window() {
    BOOST_LOG_TRIVIAL(debug) << "Window::Window()";

    resize_handler_ = [](GLFWwindow *window, int width, int height) {
        BOOST_LOG_TRIVIAL(debug) << "Window::resize_handler_(window=" << window << ", width=" << width
                                 << ", height=" << height << ")";

        glViewport(0, 0, width, height);

        Window::instance().getRenderer().draw(GL_TRIANGLES);

        BOOST_LOG_TRIVIAL(trace) << "Window::resize_handler_(window=" << window << ", width=" << width
                                 << ", height=" << height << ") end";
    };

    key_handler_ = [](GLFWwindow *glfw_window, int key, int scancode, int action, int mods) {
        BOOST_LOG_TRIVIAL(debug) << "window::key_handler_(window=" << glfw_window << ", key=" << key
                                 << ", scancode=" << scancode << ", mods=" << mods << ")";

        auto &window = Window::instance();
        if (glfw_window != window.glfw_window_) {
            BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << glfw_window << " against " << window.glfw_window_;
            return;
        }

        const auto existing_action = window.action_map_.find(key);
        if (window.action_map_.end() != existing_action) {
            existing_action->second(action);
        }

        BOOST_LOG_TRIVIAL(trace) << "window::key_handler_(window=" << glfw_window << ", key=" << key
                                 << ", scancode=" << scancode << ", mods=" << mods << ") end";
    };

    setRefreshRate(60); // NOLINT(*-magic-numbers)
    BOOST_LOG_TRIVIAL(trace) << "Window::Window() end";
}

Window::~Window() {
    if (nullptr != glfw_window_) {
        glfwDestroyWindow(glfw_window_);
    }
}

Window &Window::instance() {
    static Window static_instance;
    return static_instance;
}

Renderer &Window::getRenderer() {
    assert(renderer_);
    return *renderer_;
}

void Window::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug) << "Window::init(width=" << width << ", height=" << height << ", title=" << title << ")";

    assert(!initialized_);

    glfw_window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (nullptr == glfw_window_) {
        BOOST_LOG_TRIVIAL(trace) << "Window::Window(width=" << width << ", height=" << height << ", title=" << title
                                 << ") GlfwError";
        throw GlfwError("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window_);

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        BOOST_LOG_TRIVIAL(trace) << "Window::init(width=" << width << ", height=" << height << ", title=" << title
                                 << ") GladError";
        throw GladError("gladLoadGLLoader() failed");
    }

    if (nullptr != glfwSetFramebufferSizeCallback(glfw_window_, resize_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    if (nullptr != glfwSetKeyCallback(glfw_window_, key_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }

    glViewport(0, 0, width, height);

    renderer_ = std::make_unique<Renderer>();

    initialized_ = true;

    BOOST_LOG_TRIVIAL(trace) << "Window::init(width=" << width << ", height=" << height << ", title=" << title
                             << ") end";
}

void Window::renderLoop() noexcept {
    BOOST_LOG_TRIVIAL(debug) << "Window::renderLoop(glfw_window_=" << glfw_window_ << ")";

    assert(initialized_);

    while (1 != glfwWindowShouldClose(glfw_window_)) {
        auto start_time = std::chrono::high_resolution_clock::now();

        glfwSwapBuffers(glfw_window_);
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

    BOOST_LOG_TRIVIAL(trace) << "Window::renderLoop(glfw_window_=" << glfw_window_ << ") end";
}

void Window::setKeyAction(int key, Action action) noexcept {
    auto existing_action = action_map_.find(key);
    if (action_map_.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = std::move(action);
    } else {
        action_map_[key] = std::move(action);
    }
}

void Window::setWindowShouldClose(int value) noexcept {
    assert(initialized_);
    glfwSetWindowShouldClose(glfw_window_, value);
}

void Window::setRefreshRate(int refresh_rate) noexcept {
    constexpr auto s_to_us_multiplier = 1000000.0;
    assert(refresh_rate > 0);
    frame_time_us_ = s_to_us_multiplier / refresh_rate;
}

} // namespace opengl_wrapper