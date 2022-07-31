#ifndef OPENGL_WRAPPER_WINDOW_H
#define OPENGL_WRAPPER_WINDOW_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace opengl_wrapper {

class window {
  public:
    /**
     * @brief Creates a window object
     *
     * @param width Window width
     * @param height Window height
     * @param title Window title
     *
     * @throws GladError When the initialization of GLAD fails
     */
    window(int width, int height, const char *title);

    /**
     * @brief Engages the window render loop
     */
    void render_loop() noexcept;

  private:
    static GLFWframebuffersizefun resize_handler;
    GLFWwindow *glfw_window_;
};

} // namespace opengl_wrapper

#endif
