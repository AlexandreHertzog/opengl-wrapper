#pragma once

#include "opengl-wrapper/data_types/camera.h"
#include "opengl-wrapper/data_types/window.h"
#include "opengl-wrapper/graphics/graphics.h"
#include <functional>
#include <map>
#include <memory>

namespace opengl_wrapper {

class renderer;

class window_manager {
  public:
    /**
     * @brief The action to be ran on a given keypress
     *
     * @param int GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
     *
     */
    using key_callback_t = std::function<void(int)>;

    /**
     * @brief The action to be ran on a cursor position update.
     *
     * @param double x position
     * @param double y position
     */
    using cursor_pos_callback_t = std::function<void(double, double)>;

    window_manager();
    ~window_manager() = default;
    window_manager(const window_manager &) = delete;
    window_manager(window_manager &&) = delete;
    window_manager &operator=(const window_manager &) = delete;
    window_manager &operator=(window_manager &&) = delete;

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
    void set_key_action(int key, key_callback_t action) noexcept;

    /**
     * @brief Sets the callback for when the cursor moves.
     * @param callback Callback to be called.
     */
    void set_cursor_position_callback(cursor_pos_callback_t callback);

    /**
     * @brief Set the window_manager Should Close object. Requires init() to be called
     * beforehand.
     *
     * @param value
     */
    void set_window_should_close(int value) noexcept;

    /**
     * @brief Enables or disables the cursor. Disabling means capturing the cursor and hiding it.
     * @param enabled
     */
    void set_cursor_enabled(bool enabled);

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

    camera &get_camera();

  private:
    static std::map<const void *, window_manager *> m_windows_map;
    GLFWframebuffersizefun m_resize_handler;
    GLFWkeyfun m_key_handler;
    GLFWcursorposfun m_cursor_pos_handler;
    std::map<int, key_callback_t> m_action_map;
    bool m_initialized;
    double m_frame_time_us;
    camera m_camera;

    std::unique_ptr<window> m_window;
    std::unique_ptr<renderer> m_renderer;
    std::unique_ptr<cursor_pos_callback_t> m_app_cursor_pos_callback;
};

} // namespace opengl_wrapper
