#ifndef OPENGL_WRAPPER_WINDOW_MANAGER_H
#define OPENGL_WRAPPER_WINDOW_MANAGER_H

#include <map>
#include <memory>

#include "window.h"

namespace opengl_wrapper {

class WindowManager {
  public:
    using WindowReference = std::shared_ptr<Window>;

    /**
     * @brief Gets the singleton instance. Makes sure it is initialized.
     *
     * @return WindowManager& Instance.
     */
    static WindowManager &instance() noexcept;

    ~WindowManager() = default;

    /**
     * @brief Create a Window object and return its reference.
     *
     * @param width Window width
     * @param height Window height
     * @param title Window title
     * @return WindowReference Created window reference
     */
    WindowReference createWindow(int width, int height,
                                 const char *title) noexcept;

    /**
     * @brief Gets the Window by GLFW window pointer
     *
     * @param glfw_window GLFW window pointer
     * @return WindowReference
     */
    WindowReference getWindowByGlfwWindow(void *glfw_window);
    void unregisterWindow(WindowReference window) noexcept;

  private:
    std::map<void *, WindowReference> window_map_;

    WindowManager() = default;
    WindowManager(const WindowManager &) = delete;
    WindowManager(WindowManager &&) = delete;
    WindowManager &operator=(const WindowManager &) = delete;
    WindowManager &operator=(WindowManager &&) = delete;
};

} // namespace opengl_wrapper

#endif
