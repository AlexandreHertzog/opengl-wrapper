#include "window.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/glfw_error.h"
#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <iomanip>

namespace opengl_wrapper {
window::window(int width, int height, const char *title)
    : m_window(graphics::instance().glfw_create_window(width, height, title, nullptr, nullptr)) {

    BOOST_LOG_TRIVIAL(trace) << "window::window " << *this << " width=" << width << " height=" << height
                             << " title=" << std::quoted(title);

    if (nullptr == m_window) {
        BOOST_LOG_TRIVIAL(trace) << "window::window glfw_error";
        throw glfw_error("glfwCreateWindow() failed");
    }
}

window::window(window &&other) noexcept : m_window(other.m_window) {
    BOOST_LOG_TRIVIAL(trace) << "window::window " << *this << " other=" << other;
    other.m_window = nullptr;
}

window::~window() {
    BOOST_LOG_TRIVIAL(trace) << "window::~window " << *this;
    if (nullptr != m_window) {
        graphics::instance().glfw_destroy_window(m_window);
    }
}

window &window::operator=(window &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "window::operator= " << *this << " other=" << other;
    this->m_window = other.m_window;
    other.m_window = nullptr;
    return *this;
}

bool window::operator==(GLFWwindow *other) const {
    BOOST_LOG_TRIVIAL(trace) << "window::operator== " << *this << " other=" << other;
    return this->m_window == other;
}

bool window::operator!=(GLFWwindow *other) const {
    BOOST_LOG_TRIVIAL(trace) << "window::operator!= " << *this << " other=" << other;
    return !(*this == other);
}

void window::set_as_context() {
    BOOST_LOG_TRIVIAL(trace) << "window::set_as_context " << *this;
    assert(m_window != nullptr);
    graphics::instance().glfw_make_context_current(m_window);
}

void window::set_framebuffer_callback(GLFWframebuffersizefun fun) {
    BOOST_LOG_TRIVIAL(trace) << "window::set_framebuffer_callback " << *this << " fun=" << &fun;
    assert(m_window != nullptr);
    if (nullptr != graphics::instance().glfw_set_framebuffer_size_callback(m_window, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous resize callback";
    }
}

void window::set_key_callback(GLFWkeyfun fun) {
    BOOST_LOG_TRIVIAL(trace) << "window::set_key_callback " << *this << " fun=" << &fun;
    assert(m_window != nullptr);
    if (nullptr != graphics::instance().glfw_set_key_callback(m_window, fun)) {
        BOOST_LOG_TRIVIAL(info) << "Overwriting previous key callback";
    }
}

void window::set_should_close(int should_close) {
    BOOST_LOG_TRIVIAL(trace) << "window::set_should_close " << *this << " should_close=" << should_close;
    assert(m_window != nullptr);
    graphics::instance().glfw_set_window_should_close(m_window, should_close);
}

int window::get_should_close() const {
    BOOST_LOG_TRIVIAL(trace) << "window::get_should_close " << *this;
    assert(m_window != nullptr);
    return graphics::instance().glfw_window_should_close(m_window);
}

void window::swap_buffers() {
    BOOST_LOG_TRIVIAL(trace) << "window::swap_buffers " << *this;
    assert(m_window != nullptr);
    graphics::instance().glfw_swap_buffers(m_window);
}

const GLFWwindow *window::get_window() const {
    return m_window;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window &w) {
    return os << "window" << parenthesis(&w) << " window=" << w.get_window();
}

} // namespace opengl_wrapper
