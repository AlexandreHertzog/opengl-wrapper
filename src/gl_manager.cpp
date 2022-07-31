#include "gl_manager.h"

#include <boost/log/trivial.hpp>
#include <iomanip>
#include <iostream>

#include "exceptions/glad_error.h"
#include "exceptions/glfw_error.h"
#include "utils/formatter.h"

namespace opengl_wrapper {

GLFWerrorfun gl_manager::error_handler = [](int error_code,
                                            const char *description) {
    BOOST_LOG_TRIVIAL(error) << "GLFW error: error_code="
                             << Formatter::format_glfw_error_code(error_code)
                             << ", description=\"" << description << "\"";
};

GLFWframebuffersizefun gl_manager::resize_handler = [](GLFWwindow *window,
                                                       int width, int height) {
    BOOST_LOG_TRIVIAL(debug)
        << "gl_manager::resize_handler(window=" << window << ", width=" << width
        << ", height=" << height << ")";

    glViewport(0, 0, width, height);

    BOOST_LOG_TRIVIAL(trace)
        << "gl_manager::resize_handler(window=" << window << ", width=" << width
        << ", height=" << height << ") end";
};

gl_manager::gl_manager() {
    BOOST_LOG_TRIVIAL(debug) << "gl_manager::gl_manager()";
    if (NULL != glfwSetErrorCallback(gl_manager::error_handler)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous error callback";
    }

    if (GLFW_TRUE != glfwInit()) {
        BOOST_LOG_TRIVIAL(trace) << "gl_manager::gl_manager() glfw_error";
        throw glfw_error("glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    BOOST_LOG_TRIVIAL(info) << "gl_manager started successfully";
    BOOST_LOG_TRIVIAL(trace) << "gl_manager::gl_manager() end";
}

gl_manager::~gl_manager() {
    BOOST_LOG_TRIVIAL(debug) << "gl_manager::~gl_manager()";
    glfwTerminate();
    BOOST_LOG_TRIVIAL(trace) << "gl_manager::~gl_manager() end";
}

gl_manager &gl_manager::instance() {
    static gl_manager static_instance;
    return static_instance;
}

GLFWwindow *gl_manager::display_window(int width, int height,
                                       const char *title) {
    BOOST_LOG_TRIVIAL(debug)
        << "gl_manager::display_window(width=" << width << ", height=" << height
        << ", title=" << title << ")";

    auto window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (NULL == window) {
        BOOST_LOG_TRIVIAL(trace)
            << "gl_manager::display_window(width=" << width
            << ", height=" << height << ", title=" << title << ") glfw_error";
        throw glfw_error("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        BOOST_LOG_TRIVIAL(trace)
            << "gl_manager::display_window(width=" << width
            << ", height=" << height << ", title=" << title << ") glad_error";
        throw glad_error("gladLoadGLLoader() failed");
    }

    if (glfwSetFramebufferSizeCallback(window, resize_handler)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }

    glViewport(0, 0, width, height);
    BOOST_LOG_TRIVIAL(trace)
        << "gl_manager::display_window(width=" << width << ", height=" << height
        << ", title=" << title << ") end";
    return window;
}

void gl_manager::render_loop(GLFWwindow *window) noexcept {
    BOOST_LOG_TRIVIAL(debug)
        << "gl_manager::render_loop(window=" << window << ")";

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    BOOST_LOG_TRIVIAL(trace)
        << "gl_manager::render_loop(window=" << window << ") end";
}

} // namespace opengl_wrapper
