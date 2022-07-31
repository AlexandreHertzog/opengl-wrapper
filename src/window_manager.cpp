#include "window_manager.h"

#include <boost/log/trivial.hpp>
#include <cassert>

#include "exceptions/unregistered_window_error.h"
#include "utils/formatter.h"

namespace opengl_wrapper {
WindowManager &WindowManager::instance() noexcept {
    static WindowManager static_instance;
    return static_instance;
}

WindowManager::WindowReference
WindowManager::createWindow(int width, int height, const char *title) noexcept {
    std::shared_ptr<Window> window(new Window(width, height, title));
    window_map_[window->getWindow()] = window;
    return window;
}

WindowManager::WindowReference
WindowManager::getWindowByGlfwWindow(void *glfw_window) {
    auto existing_window = window_map_.find(glfw_window);
    if (window_map_.end() == existing_window) {
        throw UnregisteredWindowError(Formatter::formatGlfwWindow(
            reinterpret_cast<GLFWwindow *>(glfw_window)));
    }
    return existing_window->second;
}

void WindowManager::unregisterWindow(WindowReference window) noexcept {
    assert(window);
    auto existing_window = window_map_.find(window->getWindow());
    if (window_map_.end() == existing_window) {
        BOOST_LOG_TRIVIAL(warning)
            << "Window " << window->getWindow() << " not found";
        return;
    }

    window_map_.erase(existing_window);
}
} // namespace opengl_wrapper