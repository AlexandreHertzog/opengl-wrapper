#ifndef OPENGL_WRAPPER_WINDOW_MANAGER_H
#define OPENGL_WRAPPER_WINDOW_MANAGER_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <memory>

#include "opengl-wrapper/wraps/window.h"
#include "renderer.h"

namespace opengl_wrapper {

class WindowManager {
  public:
    /**
     * @brief The action to be ran on a given keypress
     *
     * @param int GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
     *
     */
    using Action = std::function<void(int)>;

    ~WindowManager() = default;
    WindowManager(const WindowManager &) = delete;
    WindowManager(WindowManager &&) = delete;
    WindowManager &operator=(const WindowManager &) = delete;
    WindowManager &operator=(WindowManager &&) = delete;

    /**
     * @brief Returns the static window instance.
     *
     * @return WindowManager& Static window instance.
     */
    static WindowManager &instance();

    /**
     * @brief Returns the renderer associated with this window.
     *
     * @return Renderer reference.
     */
    Renderer &getRenderer();

    /**
     * @brief Initializes the static window.
     *
     * @param width WindowManager width.
     * @param height WindowManager height.
     * @param title WindowManager title.
     */
    void init(int width, int height, const char *title);

    /**
     * @brief Set the given callback to be executed when key is pressed/released
     *
     * @param key GLFW respective key
     * @param action Action to be ran upon key
     */
    void setKeyAction(int key, Action action) noexcept;

    /**
     * @brief Set the WindowManager Should Close object. Requires init() to be called
     * beforehand.
     *
     * @param value
     */
    void setWindowShouldClose(int value) noexcept;

    /**
     * @brief Sets the window refresh rate, in hertz.
     *
     * @param refresh_rate Refresh rate in hertz.
     */
    void setRefreshRate(int refresh_rate) noexcept;

    /**
     * @brief Engages the WindowManager render loop. Requires init() to be called
     * beforehand.
     */
    void renderLoop() noexcept;

  private:
    GLFWframebuffersizefun resize_handler_;
    GLFWkeyfun key_handler_;
    std::map<int, Action> action_map_;
    bool initialized_{};
    double frame_time_us_{};

    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;

    WindowManager();
};

} // namespace opengl_wrapper

#endif
