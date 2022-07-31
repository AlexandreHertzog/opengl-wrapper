#include "window.h"

#include <boost/log/trivial.hpp>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"

namespace opengl_wrapper {

GLFWframebuffersizefun Window::resize_handler_ = [](GLFWwindow *window,
                                                    int width, int height) {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::resize_handler_(window=" << window << ", width=" << width
        << ", height=" << height << ")";

    glViewport(0, 0, width, height);

    BOOST_LOG_TRIVIAL(trace)
        << "Window::resize_handler_(window=" << window << ", width=" << width
        << ", height=" << height << ") end";
};

Window::Window(int width, int height, const char *title) {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::Window(width=" << width << ", height=" << height
        << ", title=" << title << ")";

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
            << "Window::Window(width=" << width << ", height=" << height
            << ", title=" << title << ") GladError";
        throw GladError("gladLoadGLLoader() failed");
    }

    if (glfwSetFramebufferSizeCallback(glfw_window_, resize_handler_)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    glViewport(0, 0, width, height);
    BOOST_LOG_TRIVIAL(trace)
        << "Window::Window(width=" << width << ", height=" << height
        << ", title=" << title << ") end";
}

void Window::renderLoop() noexcept {
    BOOST_LOG_TRIVIAL(debug)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ")";

    while (!glfwWindowShouldClose(glfw_window_)) {
        glfwSwapBuffers(glfw_window_);
        glfwPollEvents();
    }

    BOOST_LOG_TRIVIAL(trace)
        << "Window::renderLoop(glfw_window_=" << glfw_window_ << ") end";
}

} // namespace opengl_wrapper