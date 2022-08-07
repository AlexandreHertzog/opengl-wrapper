#ifndef OPENGL_WRAPPER_WINDOW_H
#define OPENGL_WRAPPER_WINDOW_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <functional>
#include <map>

namespace opengl_wrapper {

class Window {
  public:
    /**
     * @brief The action to be ran on a given keypress
     *
     * @param int GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
     *
     */
    using Action = std::function<void(int)>;

    /**
     * @brief Returns the static window instance.
     *
     * @return Window& Static window instance.
     */
    static Window &instance();

    /**
     * @brief Initializes the static window.
     *
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
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
     * @brief Set the Window Should Close object. Requires init() to be called
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
     * @brief Engages the Window render loop. Requires init() to be called
     * beforehand.
     */
    void renderLoop() noexcept;

  private:
    GLFWframebuffersizefun resize_handler_;
    GLFWkeyfun key_handler_;
    GLFWwindow *glfw_window_;
    std::map<int, Action> action_map_;
    bool initialized_;
    double frame_time_ms_;

    friend class WindowManager;

    Window();
    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
};

} // namespace opengl_wrapper

#endif
