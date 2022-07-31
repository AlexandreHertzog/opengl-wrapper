#ifndef OPENGL_WRAPPER_WINDOW_H
#define OPENGL_WRAPPER_WINDOW_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace opengl_wrapper {

class Window {
  public:
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

    /**
     * @brief Engages the Window render loop
     */
    void renderLoop() noexcept;

  private:
    static GLFWframebuffersizefun resize_handler_;
    GLFWwindow *glfw_window_;
};

} // namespace opengl_wrapper

#endif
