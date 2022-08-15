#include "window.h"

#include <boost/log/trivial.hpp>
#include <cassert>

#include "exceptions/glfw_error.h"

namespace opengl_wrapper {
window::window(int width, int height, const char *title)
    : window_(glfwCreateWindow(width, height, title, nullptr, nullptr)) {

    if (nullptr == window_) {
        BOOST_LOG_TRIVIAL(trace) << "window_manager::window_manager(width=" << width << ", height=" << height
                                 << ", title=" << title << ") glfw_error";
        throw glfw_error("glfwCreateWindow() failed");
    }
}

window::window(window &&other) noexcept : window_(other.window_) {
    other.window_ = nullptr;
}

window::~window() {
    if (nullptr != window_) {
        glfwDestroyWindow(window_);
    }
}

window &window::operator=(window &&other) noexcept {
    this->window_ = other.window_;
    other.window_ = nullptr;
    return *this;
}

bool window::operator==(GLFWwindow *other) const {
    return this->window_ == other;
}

bool window::operator!=(GLFWwindow *other) const {
    return !(*this == other);
}

void window::set_as_context() {
    assert(window_ != nullptr);
    glfwMakeContextCurrent(window_);
}

void window::set_framebuffer_callback(GLFWframebuffersizefun fun) {
    assert(window_ != nullptr);
    if (nullptr != glfwSetFramebufferSizeCallback(window_, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }
}

void window::set_key_callback(GLFWkeyfun fun) {
    assert(window_ != nullptr);
    if (nullptr != glfwSetKeyCallback(window_, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }
}

void window::set_should_close(int should_close) {
    assert(window_ != nullptr);
    glfwSetWindowShouldClose(window_, should_close);
}

int window::get_should_close() const {
    assert(window_ != nullptr);
    return glfwWindowShouldClose(window_);
}

void window::swap_buffers() {
    assert(window_ != nullptr);
    glfwSwapBuffers(window_);
}
} // namespace opengl_wrapper