#ifndef OPENGL_WRAPPER_WRAPS_WINDOW_H
#define OPENGL_WRAPPER_WRAPS_WINDOW_H

#include "opengl-wrapper/graphics/graphics.h"
#include <functional>
#include <ostream>

namespace opengl_wrapper {

class window {
  public:
    /**
     * @brief Constructs a window object.
     * @param width window width.
     * @param height window height.
     * @param title window title.
     */
    window(int width, int height, const char *title);

    /**
     * @brief window move-constructor.
     * @param other window to be moved.
     */
    window(window &&other) noexcept;

    /**
     * @brief window destructor.
     */
    ~window();

    /**
     * @brief window move-assignment operator.
     * @param other window to be moved.
     * @return Reference to lhs.
     */
    window &operator=(window &&other) noexcept;

    window(const window &) = delete;
    window &operator=(const window &) = delete;

    /**
     * @brief window equality operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow refers to this object.
     */
    bool operator==(GLFWwindow *other) const;

    /**
     * @brief window difference operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow does not refer to this object.
     */
    bool operator!=(GLFWwindow *other) const;

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
    void set_framebuffer_callback(GLFWframebuffersizefun fun);

    /**
     * @brief Sets the key-pressed callback function. See
     * https://www.glfw.org/docs/3.0/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
     * @param fun Function to be called whenever a key is pressed.
     */
    void set_key_callback(GLFWkeyfun fun);

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
    [[nodiscard]] int get_should_close() const;

    /**
     * Swaps the front and the back buffers for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     */
    void swap_buffers();

    [[nodiscard]] const GLFWwindow *get_window() const;

  private:
    GLFWwindow *window_;
};

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::window &w);

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_WRAPS_WINDOW_H
