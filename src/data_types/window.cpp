#include "window.h"

#include "graphics/graphics.h"
#include "utils/glad_error.h"
#include "utils/glfw_error.h"
#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <iomanip>
#include <map>

namespace opengl_wrapper {

class window::manager {
  public:
    static std::map<const void *, opengl_wrapper::window *> m_window_map; // NOLINT(*-non-const-global-variables)

    static void callback_cursor_pos(GLFWwindow *glfw_window, double xpos, double ypos) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        if (window.m_cursor_pos_callback) {
            window.m_cursor_pos_callback(window, xpos, ypos);
        }
    }

    static void callback_framebuffer_size(GLFWwindow *glfw_window, int width, int height) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_framebuffer_callback(window, width, height);
    }

    static void callback_key(GLFWwindow *glfw_window, int key, int scancode, int action, int mods) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_key_callback(window, key, scancode, action, mods);
    }
};

std::map<const void *, opengl_wrapper::window *> window::manager::m_window_map; // NOLINT(*-non-const-global-variables)

window::window(int width, int height, const char *title)
    : m_window(graphics::instance().glfw_create_window(width, height, title, nullptr, nullptr)) {

    if (nullptr == m_window) {
        throw glfw_error("glfwCreateWindow() failed");
    }

    set_as_context();

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { //  NOLINT(*-reinterpret-cast)
        throw glad_error("gladLoadGLLoader() failed");
    }

    manager::m_window_map[m_window] = this;
    register_callbacks();
}

window::window(window &&other) noexcept : m_window(other.m_window) {
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
}

window::~window() {
    manager::m_window_map.erase(m_window);

    if (nullptr != m_window) {
        graphics::instance().glfw_destroy_window(m_window);
    }
}

window &window::operator=(window &&other) noexcept {
    this->m_window = other.m_window;
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
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

void window::set_framebuffer_callback(window::framebuffer_cb_t fun) {
    m_framebuffer_callback = std::move(fun);
}

void window::set_key_callback(window::key_cb_t fun) {
    m_key_callback = std::move(fun);
}

void window::set_cursor_pos_callback(window::cursor_pos_cb_t fun) {
    m_cursor_pos_callback = std::move(fun);
}

void window::set_should_close(int should_close) {
    assert(m_window != nullptr);
    graphics::instance().glfw_set_window_should_close(m_window, should_close);
}

bool window::get_should_close() const {
    assert(m_window != nullptr);
    return graphics::instance().glfw_window_should_close(m_window) != 0;
}

void window::swap_buffers() {
    assert(m_window != nullptr);
    graphics::instance().glfw_swap_buffers(m_window);
}

void window::poll_events() {
    assert(m_window != nullptr);
    graphics::instance().glfw_poll_events();
}

void window::set_input_mode(int mode, int value) {
    assert(nullptr != m_window);
    graphics::instance().glfw_set_input_mode(m_window, mode, value);
}

GLFWwindow *window::get_window() const {
    return m_window;
}

void window::register_callbacks() {
    graphics::instance().glfw_set_framebuffer_size_callback(m_window, manager::callback_framebuffer_size);
    graphics::instance().glfw_set_key_callback(m_window, manager::callback_key);
    graphics::instance().glfw_set_cursor_pos_callback(m_window, manager::callback_cursor_pos);
}

void window::set_viewport(size_t width, size_t height) { // NOLINT(readability-convert-member-functions-to-static)
    graphics::instance().set_viewport(width, height);
}

void window::set_wireframe_mode(bool enable) { // NOLINT(readability-convert-member-functions-to-static)
    if (enable) {
        graphics::instance().polygon_mode(polygon_mode_t::line);
    } else {
        graphics::instance().polygon_mode(polygon_mode_t::fill);
    }
}

void window::set_depth_test(bool enable) { // NOLINT(readability-convert-member-functions-to-static)
    if (enable) {
        graphics::instance().enable(graphics_feature_t::depth_test);
    } else {
        graphics::instance().disable(graphics_feature_t::depth_test);
    }
}

void window::set_clear_color(const color_alpha_t &c) {
    graphics::instance().set_clear_color(c);
}

void window::clear() {
    graphics::instance().clear();
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window &w) {
    return os << "window" << parenthesis(&w) << " window=" << w.get_window();
}

} // namespace opengl_wrapper
