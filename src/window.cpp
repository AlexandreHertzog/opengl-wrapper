#include "window.h"

#include <boost/log/trivial.hpp>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"

namespace opengl_wrapper {

GLFWframebuffersizefun window::resize_handler = [](GLFWwindow *window,
                                                   int width, int height) {
    BOOST_LOG_TRIVIAL(debug)
        << "window::resize_handler(window=" << window << ", width=" << width
        << ", height=" << height << ")";

    glViewport(0, 0, width, height);

    BOOST_LOG_TRIVIAL(trace)
        << "window::resize_handler(window=" << window << ", width=" << width
        << ", height=" << height << ") end";
};

window::window(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug)
        << "window::window(width=" << width << ", height=" << height
        << ", title=" << title << ")";

    glfw_window_ = glfwCreateWindow(width, height, title, NULL, NULL);
    if (NULL == glfw_window_) {
        BOOST_LOG_TRIVIAL(trace)
            << "window::window(width=" << width << ", height=" << height
            << ", title=" << title << ") glfw_error";
        throw glfw_error("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        BOOST_LOG_TRIVIAL(trace)
            << "window::window(width=" << width << ", height=" << height
            << ", title=" << title << ") glad_error";
        throw glad_error("gladLoadGLLoader() failed");
    }

    if (glfwSetFramebufferSizeCallback(glfw_window_, resize_handler)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    glViewport(0, 0, width, height);
    BOOST_LOG_TRIVIAL(trace)
        << "window::window(width=" << width << ", height=" << height
        << ", title=" << title << ") end";
}

void window::render_loop() noexcept {
    BOOST_LOG_TRIVIAL(debug)
        << "window::render_loop(glfw_window_=" << glfw_window_ << ")";

    while (!glfwWindowShouldClose(glfw_window_)) {
        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
    }

    BOOST_LOG_TRIVIAL(trace)
        << "window::render_loop(glfw_window_=" << glfw_window_ << ") end";
}

} // namespace opengl_wrapper