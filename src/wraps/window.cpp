#include "window.h"

#include <boost/log/trivial.hpp>
#include <cassert>

#include "exceptions/glfw_error.h"

namespace opengl_wrapper {
Window::Window(int width, int height, const char *title)
    : window_(glfwCreateWindow(width, height, title, nullptr, nullptr)) {

    if (nullptr == window_) {
        BOOST_LOG_TRIVIAL(trace) << "WindowManager::WindowManager(width=" << width << ", height=" << height
                                 << ", title=" << title << ") GlfwError";
        throw GlfwError("glfwCreateWindow() failed");
    }
}

Window::Window(Window &&other) noexcept : window_(other.window_) {
    other.window_ = nullptr;
}

Window::~Window() {
    if (nullptr != window_) {
        glfwDestroyWindow(window_);
    }
}

Window &Window::operator=(Window &&other) noexcept {
    this->window_ = other.window_;
    other.window_ = nullptr;
    return *this;
}

bool Window::operator==(GLFWwindow *other) const {
    return this->window_ == other;
}

bool Window::operator!=(GLFWwindow *other) const {
    return !(*this == other);
}

void Window::makeCurrentContext() {
    assert(window_ != nullptr);
    glfwMakeContextCurrent(window_);
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun fun) {
    assert(window_ != nullptr);
    if (nullptr != glfwSetFramebufferSizeCallback(window_, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }
}

void Window::setKeyCallback(GLFWkeyfun fun) {
    assert(window_ != nullptr);
    if (nullptr != glfwSetKeyCallback(window_, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }
}

void Window::setShouldClose(int should_close) {
    assert(window_ != nullptr);
    glfwSetWindowShouldClose(window_, should_close);
}

int Window::shouldClose() const {
    assert(window_ != nullptr);
    return glfwWindowShouldClose(window_);
}

void Window::swapBuffers() {
    assert(window_ != nullptr);
    glfwSwapBuffers(window_);
}
} // namespace opengl_wrapper