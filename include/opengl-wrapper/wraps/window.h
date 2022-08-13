#ifndef OPENGL_WRAPPER_WRAPS_WINDOW_H
#define OPENGL_WRAPPER_WRAPS_WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>

namespace opengl_wrapper {
class Window {
  public:
    /**
     * @brief Constructs a Window object.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    Window(int width, int height, const char *title);

    /**
     * @brief Window move-constructor.
     * @param other Window to be moved.
     */
    Window(Window &&other) noexcept;

    /**
     * @brief Window destructor.
     */
    ~Window();

    /**
     * @brief Window move-assignment operator.
     * @param other Window to be moved.
     * @return Reference to lhs.
     */
    Window &operator=(Window &&other) noexcept;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    /**
     * @brief Window equality operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow refers to this object.
     */
    bool operator==(GLFWwindow *other) const;

    /**
     * @brief Window difference operator with GLFWwindow instances.
     * @param other GLFWwindow to test the object against.
     * @return Whether the GLFWwindow does not refer to this object.
     */
    bool operator!=(GLFWwindow *other) const;

    /**
     * @brief Makes the Window context the one being referenced by OpenGL. See
     * https://www.glfw.org/docs/3.3/group__context.html#ga1c04dc242268f827290fe40aa1c91157
     */
    void makeCurrentContext();

    /**
     * @brief Sets the window-resizing callback function. See
     * https://www.glfw.org/docs/3.0/group__window.html#ga3203461a5303bf289f2e05f854b2f7cf
     * @param fun Function to be called whenever the window is resized.
     */
    void setFramebufferSizeCallback(GLFWframebuffersizefun fun);

    /**
     * @brief Sets the key-pressed callback function. See
     * https://www.glfw.org/docs/3.0/group__input.html#ga7e496507126f35ea72f01b2e6ef6d155
     * @param fun Function to be called whenever a key is pressed.
     */
    void setKeyCallback(GLFWkeyfun fun);

    /**
     * @brief Sets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
     * @param should_close 1 if the window should be closed, 0 otherwise.
     */
    void setShouldClose(int should_close);

    /**
     * @brief Gets the close flag for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
     * @return 1 if the window should be closed, 0 otherwise.
     */
    [[nodiscard]] int shouldClose() const;

    /**
     * Swaps the front and the back buffers for the window. See
     * https://www.glfw.org/docs/3.3/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     */
    void swapBuffers();

  private:
    GLFWwindow *window_;
};
} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_WRAPS_WINDOW_H
