#include "window.h"

#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <iomanip>
#include <map>

namespace game_engine {

class window_t::manager {
  public:
    static std::map<const void *, game_engine::window_t *> m_window_map; // NOLINT(*-non-const-global-variables)

    static void callback_cursor_pos(GLFWwindow *glfw_window, double position_x, double position_y) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        if (window.m_cursor_pos_callback) {
            window.m_cursor_pos_callback(position_x, position_y);
        }
    }

    static void callback_framebuffer_size(GLFWwindow *glfw_window, int width, int height) {
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_framebuffer_callback(width, height);
    }

    static void callback_key(GLFWwindow *glfw_window, int key, int, int action, int) { // NOLINT(hicpp-named-parameter)
        auto window_it = m_window_map.find(glfw_window);
        assert(m_window_map.end() != window_it);
        assert(nullptr != window_it->second);

        auto &window = *window_it->second;
        window.m_key_callback(key, action);
    }
};

std::map<const void *, game_engine::window_t *> window_t::manager::m_window_map; // NOLINT(*-non-const-global-variables)

window_t::window_t(opengl_cpp::glfw_t &glfw, opengl_cpp::gl_t &gl, int width, int height, const char *title)
    : m_glfw(glfw), m_gl(gl), m_window(m_glfw.create_window(width, height, title, nullptr, nullptr)) {

    if (nullptr == m_window) {
        throw exception_t("glfwCreateWindow() failed");
    }

    set_as_context();
    m_glfw.load_gl_loader();

    manager::m_window_map[m_window] = this;
    register_callbacks();
}

window_t::window_t(window_t &&other) noexcept : m_glfw(other.m_glfw), m_gl(other.m_gl), m_window(other.m_window) {
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
}

window_t::~window_t() {
    manager::m_window_map.erase(m_window);

    if (nullptr != m_window) {
        m_glfw.destroy_window(m_window);
    }
}

window_t &window_t::operator=(window_t &&other) noexcept {
    this->m_window = other.m_window;
    other.m_window = nullptr;
    manager::m_window_map[m_window] = this;
    return *this;
}

void window_t::set_as_context() {
    assert(m_window != nullptr);
    m_glfw.make_context_current(m_window);
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
    m_glfw.set_window_should_close(m_window, should_close);
}

bool window_t::get_should_close() const {
    assert(m_window != nullptr);
    return m_glfw.window_should_close(m_window) != 0;
}

void window_t::swap_buffers() {
    assert(m_window != nullptr);
    m_glfw.swap_buffers(m_window);
}

void window_t::poll_events() {
    assert(m_window != nullptr);
    m_glfw.poll_events();
}

void window_t::set_input_mode(int mode, int value) {
    assert(nullptr != m_window);
    m_glfw.set_input_mode(m_window, mode, value);
}

GLFWwindow *window_t::get_window() const {
    return m_window;
}

void window_t::register_callbacks() {
    m_glfw.set_framebuffer_size_callback(m_window, manager::callback_framebuffer_size);
    m_glfw.set_key_callback(m_window, manager::callback_key);
    m_glfw.set_cursor_pos_callback(m_window, manager::callback_cursor_pos);
}

std::ostream &operator<<(std::ostream &os, const game_engine::window_t &w) {
    return os << "window" << parenthesis(&w) << " window=" << w.get_window();
}

} // namespace game_engine
