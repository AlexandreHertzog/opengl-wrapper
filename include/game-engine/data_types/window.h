#pragma once

#include <cassert>
#include <functional>
#include <opengl-cpp/program.h>
#include <opengl-cpp/texture.h>
#include <ostream>

#include <opengl-cpp/backend/glfw.h>

namespace game_engine {

class shape_t;

class window_t {
  public:
    using cursor_pos_cb_t = std::function<void(double, double)>;
    using framebuffer_cb_t = std::function<void(window_t &, int, int)>;
    using key_cb_t = std::function<void(window_t &, int, int)>;

    /**
     * @brief Constructs a window object.
     * @param width window width.
     * @param height window height.
     * @param title window title.
     */
    window_t(opengl_cpp::glfw_t &glfw, opengl_cpp::gl_t &gl, int width, int height, const char *title);

    /**
     * @brief window move-constructor.
     * @param other window to be moved.
     */
    window_t(window_t &&other) noexcept;

    /**
     * @brief window destructor.
     */
    ~window_t();

    /**
     * @brief window move-assignment operator.
     * @param other window to be moved.
     * @return Reference to lhs.
     */
    window_t &operator=(window_t &&other) noexcept;

    window_t(const window_t &) = delete;
    window_t &operator=(const window_t &) = delete;

    /**
     * @brief Makes the window context the one being referenced by OpenGL. See
     * https://www.glfw.org/docs/3.3/group__context.html#ga1c04dc242268f827290fe40aa1c91157
     */
    void set_as_context();

    /**
     * @brief Sets the window-resizing callback function. See
     * https://www.glfw.org/docs/3.0/group__window.html#ga3203461a5303bf289f2e05f854b2f7cf
     * @param fun Function to be called whenever the window is resized.
     */
    void set_framebuffer_callback(framebuffer_cb_t fun);

    /**
     * @brief Sets the key-pressed callback function. See
     * https://www.glfw.org/docs/3.0/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
     * @param fun Function to be called whenever a key is pressed.
     */
    void set_key_callback(key_cb_t fun);

    /**
     * @brief This function sets the cursor position callback of the specified window, which is called when the cursor
     * is moved. See https://www.glfw.org/docs/3.3/group__input.html#gac1f879ab7435d54d4d79bb469fe225d7
     * @param fun The new callback, or NULL to remove the currently set callback.
     */
    void set_cursor_pos_callback(cursor_pos_cb_t fun);

    /**
     * @brief Sets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
     * @param should_close 1 if the window should be closed, 0 otherwise.
     */
    void set_should_close(int should_close);

    /**
     * @brief Gets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
     * @return 1 if the window should be closed, 0 otherwise.
     */
    [[nodiscard]] bool get_should_close() const;

    /**
     * Swaps the front and the back buffers for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     */
    void swap_buffers();

    void poll_events();

    /**
     * @brief This function sets an input mode option for the specified window. See
     * https://www.glfw.org/docs/3.3/group__input.html#gaa92336e173da9c8834558b54ee80563b
     * @param mode One of GLFW_CURSOR, GLFW_STICKY_KEYS, GLFW_STICKY_MOUSE_BUTTONS, GLFW_LOCK_KEY_MODS or
     * GLFW_RAW_MOUSE_MOTION.
     * @param value The new value of the specified input mode.
     */
    void set_input_mode(int mode, int value);

    void draw(shape_t &s);

    /**
     * @brief Sets the dimensions of the window viewport.
     * @param width Viewport width.
     * @param height Viewport height.
     */
    void set_viewport(size_t width, size_t height);

    /**
     * @brief Sets the wireframe mode the window.
     * @param enable Enable or disable wireframe mode.
     */
    void set_wireframe_mode(bool enable);

    /**
     * @brief Sets the depth test OpenGL feature.
     * @param enable Enable or disable depth test.
     */
    void set_depth_test(bool enable);

    /**
     * @brief Sets the background color.
     * @param c Color components.
     */
    void set_clear_color(const glm::vec4 &c);

    void clear();

    [[nodiscard]] GLFWwindow *get_window() const;

  private:
    opengl_cpp::gl_t &m_gl;
    opengl_cpp::glfw_t &m_glfw;
    GLFWwindow *m_window;

    cursor_pos_cb_t m_cursor_pos_callback;
    framebuffer_cb_t m_framebuffer_callback;
    key_cb_t m_key_callback;

    class manager;

    void register_callbacks();
};

std::ostream &operator<<(std::ostream &os, const game_engine::window_t &w);

} // namespace game_engine
