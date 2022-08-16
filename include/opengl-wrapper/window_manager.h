#ifndef OPENGL_WRAPPER_WINDOW_MANAGER_H
#define OPENGL_WRAPPER_WINDOW_MANAGER_H

#include "opengl-wrapper/wraps/api.h"
#include <functional>
#include <map>
#include <memory>

namespace opengl_wrapper {

class window;
class renderer;

class window_manager {
  public:
    /**
     * @brief The action to be ran on a given keypress
     *
     * @param int GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
     *
     */
    using Action = std::function<void(int)>;

    ~window_manager() = default;
    window_manager(const window_manager &) = delete;
    window_manager(window_manager &&) = delete;
    window_manager &operator=(const window_manager &) = delete;
    window_manager &operator=(window_manager &&) = delete;

    /**
     * @brief Returns the static window instance.
     *
     * @return window_manager& Static window instance.
     */
    static window_manager &instance();

    /**
     * @brief Returns the renderer associated with this window.
     *
     * @return renderer reference.
     */
    renderer &get_renderer();

    /**
     * @brief Initializes the static window.
     *
     * @param width window_manager width.
     * @param height window_manager height.
     * @param title window_manager title.
     */
    void init(int width, int height, const char *title);

    /**
     * @brief Set the given callback to be executed when key is pressed/released
     *
     * @param key GLFW respective key
     * @param action Action to be ran upon key
     */
    void set_key_action(int key, Action action) noexcept;

    /**
     * @brief Set the window_manager Should Close object. Requires init() to be called
     * beforehand.
     *
     * @param value
     */
    void set_window_should_close(int value) noexcept;

    /**
     * @brief Sets the window refresh rate, in hertz.
     *
     * @param refresh_rate Refresh rate in hertz.
     */
    void set_refresh_rate(int refresh_rate) noexcept;

    /**
     * @brief Engages the window_manager render loop. Requires init() to be called
     * beforehand.
     */
    void render_loop() noexcept;

  private:
    GLFWframebuffersizefun resize_handler_;
    GLFWkeyfun key_handler_;
    std::map<int, Action> action_map_;
    bool initialized_;
    double frame_time_us_;

    std::unique_ptr<window> window_;
    std::unique_ptr<renderer> renderer_;

    window_manager();
};

} // namespace opengl_wrapper

#endif
