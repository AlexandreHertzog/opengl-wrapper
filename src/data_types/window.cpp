#include "window.h"

#include "graphics/graphics.h"
#include "shape.h"
#include "utils/glad_error.h"
#include "utils/glfw_error.h"
#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <iomanip>
#include <map>

namespace opengl_wrapper {

class window_t::manager {
  public:
    static std::map<const void *, opengl_wrapper::window_t *> m_window_map; // NOLINT(*-non-const-global-variables)

    static void callback_cursor_pos(GLFWwindow *glfw_window, double xpos, double ypos) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        if (window.m_cursor_pos_callback) {
            window.m_cursor_pos_callback(xpos, ypos);
        }
    }

    static void callback_framebuffer_size(GLFWwindow *glfw_window, int width, int height) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_framebuffer_callback(window, width, height);
    }

    static void callback_key(GLFWwindow *glfw_window, int key, int, int action, int) { // NOLINT(hicpp-named-parameter)
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_key_callback(window, key, action);
    }
};

std::map<const void *, opengl_wrapper::window_t *>
    window_t::manager::m_window_map; // NOLINT(*-non-const-global-variables)

window_t::window_t(int width, int height, const char *title)
    : m_window(graphics_t::instance().glfw_create_window(width, height, title, nullptr, nullptr)) {

    if (nullptr == m_window) {
        throw glfw_error_t("glfwCreateWindow() failed");
    }

    set_as_context();

    if (0 == gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { // NOLINT(*-reinterpret-cast)
        throw glad_error_t("gladLoadGLLoader() failed");
    }

    manager::m_window_map[m_window] = this;
    register_callbacks();
}

window_t::window_t(window_t &&other) noexcept : m_window(other.m_window) {
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
}

window_t::~window_t() {
    manager::m_window_map.erase(m_window);

    if (nullptr != m_window) {
        graphics_t::instance().glfw_destroy_window(m_window);
    }
}

window_t &window_t::operator=(window_t &&other) noexcept {
    this->m_window = other.m_window;
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
    return *this;
}

bool window_t::operator==(GLFWwindow *other) const {
    return this->m_window == other;
}

bool window_t::operator!=(GLFWwindow *other) const {
    return !(*this == other);
}

void window_t::set_as_context() {
    assert(m_window != nullptr);
    graphics_t::instance().glfw_make_context_current(m_window);
}

void window_t::set_framebuffer_callback(window_t::framebuffer_cb_t fun) {
    m_framebuffer_callback = std::move(fun);
}

void window_t::set_key_callback(window_t::key_cb_t fun) {
    m_key_callback = std::move(fun);
}

void window_t::set_cursor_pos_callback(window_t::cursor_pos_cb_t fun) {
    m_cursor_pos_callback = std::move(fun);
}

void window_t::set_should_close(int should_close) {
    assert(m_window != nullptr);
    graphics_t::instance().glfw_set_window_should_close(m_window, should_close);
}

bool window_t::get_should_close() const {
    assert(m_window != nullptr);
    return graphics_t::instance().glfw_window_should_close(m_window) != 0;
}

void window_t::swap_buffers() {
    assert(m_window != nullptr);
    graphics_t::instance().glfw_swap_buffers(m_window);
}

void window_t::poll_events() {
    assert(m_window != nullptr);
    graphics_t::instance().glfw_poll_events();
}

void window_t::set_input_mode(int mode, int value) {
    assert(nullptr != m_window);
    graphics_t::instance().glfw_set_input_mode(m_window, mode, value);
}

void window_t::draw(opengl_wrapper::shape_t &s) {
    s.bind();
    graphics_t::instance().draw_arrays(0, s.get_mesh().get_vertices().size());
}

GLFWwindow *window_t::get_window() const {
    return m_window;
}

void window_t::register_callbacks() {
    graphics_t::instance().glfw_set_framebuffer_size_callback(m_window, manager::callback_framebuffer_size);
    graphics_t::instance().glfw_set_key_callback(m_window, manager::callback_key);
    graphics_t::instance().glfw_set_cursor_pos_callback(m_window, manager::callback_cursor_pos);
}

void window_t::set_viewport(size_t width, size_t height) {
    graphics_t::instance().set_viewport(width, height);
}

void window_t::set_wireframe_mode(bool enable) {
    if (enable) {
        graphics_t::instance().polygon_mode(polygon_mode_t::line);
    } else {
        graphics_t::instance().polygon_mode(polygon_mode_t::fill);
    }
}

void window_t::set_depth_test(bool enable) {
    if (enable) {
        graphics_t::instance().enable(graphics_feature_t::depth_test);
    } else {
        graphics_t::instance().disable(graphics_feature_t::depth_test);
    }
}

void window_t::set_clear_color(const color_alpha_t &c) {
    graphics_t::instance().set_clear_color(c);
}

void window_t::clear() {
    graphics_t::instance().clear();
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window_t &w) {
    return os << "window" << parenthesis(&w) << " window=" << w.get_window();
}

} // namespace opengl_wrapper
