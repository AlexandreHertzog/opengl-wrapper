#include "window.h"

#include "graphics/graphics.h"
#include "utils/glfw_error.h"
#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <iomanip>

namespace opengl_wrapper {
window::window(int width, int height, const char *title)
    : m_window(graphics::instance().glfw_create_window(width, height, title, nullptr, nullptr)) {

    if (nullptr == m_window) {
        throw glfw_error("glfwCreateWindow() failed");
    }
}

window::window(window &&other) noexcept : m_window(other.m_window) {
    other.m_window = nullptr;
}

window::~window() {
    if (nullptr != m_window) {
        graphics::instance().glfw_destroy_window(m_window);
    }
}

window &window::operator=(window &&other) noexcept {
    this->m_window = other.m_window;
    other.m_window = nullptr;
    return *this;
}

bool window::operator==(GLFWwindow *other) const {
    return this->m_window == other;
}

bool window::operator!=(GLFWwindow *other) const {
    return !(*this == other);
}

void window::set_as_context() {
    assert(m_window != nullptr);
    graphics::instance().glfw_make_context_current(m_window);
}

void window::set_framebuffer_callback(GLFWframebuffersizefun fun) {
    assert(m_window != nullptr);
    graphics::instance().glfw_set_framebuffer_size_callback(m_window, fun);
}

void window::set_key_callback(GLFWkeyfun fun) {
    assert(m_window != nullptr);
    graphics::instance().glfw_set_key_callback(m_window, fun);
}

void window::set_cursor_pos_callback(GLFWcursorposfun fun) {
    assert(nullptr != m_window);
    graphics::instance().glfw_set_cursor_pos_callback(m_window, fun);
}

void window::set_should_close(int should_close) {
    assert(m_window != nullptr);
    graphics::instance().glfw_set_window_should_close(m_window, should_close);
}

int window::get_should_close() const {
    assert(m_window != nullptr);
    return graphics::instance().glfw_window_should_close(m_window);
}

void window::swap_buffers() {
    assert(m_window != nullptr);
    graphics::instance().glfw_swap_buffers(m_window);
}

void window::set_input_mode(int mode, int value) {
    assert(nullptr != m_window);
    graphics::instance().glfw_set_input_mode(m_window, mode, value);
}

const GLFWwindow *window::get_window() const {
    return m_window;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window &w) {
    return os << "window" << parenthesis(&w) << " window=" << w.get_window();
}

} // namespace opengl_wrapper
