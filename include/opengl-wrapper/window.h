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
     * @brief Engages the Window render loop
     */
    void renderLoop() noexcept;

    /**
     * @brief Set the given callback to be executed when key is pressed/released
     *
     * @param key GLFW respective key
     * @param action Action to be ran upon key
     */
    void setKeyAction(int key, Action action) noexcept;

    /**
     * @brief Get the underlying GLFW window
     *
     * @return GLFWwindow* non-null pointer
     */
    GLFWwindow *getWindow();

  private:
    static GLFWframebuffersizefun resize_handler_;
    static GLFWkeyfun key_handler_;
    GLFWwindow *glfw_window_;
    std::map<int, Action> action_map_;

    friend class WindowManager;

    /**
     * @brief Creates a Window object
     *
     * @param width Window width
     * @param height Window height
     * @param title Window title
     *
     * @throws GladError When the initialization of GLAD fails
     */
    Window(int width, int height, const char *title);

    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;
};

} // namespace opengl_wrapper

#endif
