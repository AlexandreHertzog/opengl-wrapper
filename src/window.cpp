#include "window.h"

#include <boost/log/trivial.hpp>
#include <cassert>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"

namespace opengl_wrapper {

Window::Window() {
    BOOST_LOG_TRIVIAL(debug) << "Window::Window()";

    resize_handler_ = [](GLFWwindow *window, int width, int height) {
        BOOST_LOG_TRIVIAL(debug)
            << "Window::resize_handler_(window=" << window
            << ", width=" << width << ", height=" << height << ")";

        glViewport(0, 0, width, height);

        BOOST_LOG_TRIVIAL(trace)
            << "Window::resize_handler_(window=" << window
            << ", width=" << width << ", height=" << height << ") end";
    };

    key_handler_ = [](GLFWwindow *glfw_window, int key, int scancode,
                      int action, int mods) {
        BOOST_LOG_TRIVIAL(debug)
            << "window::key_handler_(window=" << glfw_window << ", key=" << key
            << ", scancode=" << scancode << ", mods=" << mods << ")";

        auto &window = Window::instance();
        if (glfw_window != window.glfw_window_) {
            BOOST_LOG_TRIVIAL(error) << "Unexpected window: " << glfw_window
                                     << " against " << window.glfw_window_;
            return;
        }

        const auto existing_action = window.action_map_.find(key);
        if (window.action_map_.end() != existing_action) {
            existing_action->second(action);
        }

        BOOST_LOG_TRIVIAL(trace)
            << "window::key_handler_(window=" << glfw_window << ", key=" << key
            << ", scancode=" << scancode << ", mods=" << mods << ") end";
    };
    BOOST_LOG_TRIVIAL(trace) << "Window::Window() end";
}

Window &Window::instance() {
    static Window static_instance;
    return static_instance;
}

void Window::init(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::init(width=" << width << ", height=" << height
        << ", title=" << title << ")";

    assert(!initialized_);

    glfw_window_ = glfwCreateWindow(width, height, title, NULL, NULL);
    if (NULL == glfw_window_) {
        BOOST_LOG_TRIVIAL(trace)
            << "Window::Window(width=" << width << ", height=" << height
            << ", title=" << title << ") GlfwError";
        throw GlfwError("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        BOOST_LOG_TRIVIAL(trace)
            << "Window::init(width=" << width << ", height=" << height
            << ", title=" << title << ") GladError";
        throw GladError("gladLoadGLLoader() failed");
    }

    if (glfwSetFramebufferSizeCallback(glfw_window_, resize_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    if (glfwSetKeyCallback(glfw_window_, key_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }

    glViewport(0, 0, width, height);

    initialized_ = true;

    BOOST_LOG_TRIVIAL(trace)
        << "Window::init(width=" << width << ", height=" << height
        << ", title=" << title << ") end";
}

void Window::renderLoop() {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ")";

    assert(initialized_);

    while (!glfwWindowShouldClose(glfw_window_)) {
        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
    }

    BOOST_LOG_TRIVIAL(trace)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ") end";
}

void Window::setKeyAction(int key, Action action) noexcept {
    auto existing_action = action_map_.find(key);
    if (action_map_.end() != existing_action) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting callback for key " << key;
        existing_action->second = action;
    } else {
        action_map_[key] = action;
    }
}

void Window::setWindowShouldClose(int value) {
    assert(initialized_);
    glfwSetWindowShouldClose(glfw_window_, value);
}

} // namespace opengl_wrapper