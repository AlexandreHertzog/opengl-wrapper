#pragma once

#include "opengl-wrapper/graphics/graphics.h"
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

    void set_wireframe_mode(bool wireframe);

  private:
    GLFWframebuffersizefun m_resize_handler;
    GLFWkeyfun m_key_handler;
    std::map<int, Action> m_action_map;
    bool m_initialized;
    double m_frame_time_us;

    std::unique_ptr<window> m_window;
    std::unique_ptr<renderer> m_renderer;

    window_manager();
};

} // namespace opengl_wrapper
