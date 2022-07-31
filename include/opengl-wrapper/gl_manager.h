#ifndef gl_manager_H
#define gl_manager_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace opengl_wrapper {

class gl_manager {
  public:
    /**
     * @brief Returns the global MainManager instance. Makes sure it is
     * initialized
     *
     * @return MainManager& MainManager instance
     *
     * @throws GlfwError When the initialization of GLFW fails
     */
    static gl_manager &instance();

    /**
     * @brief Displays a window
     *
     * @param width Window width
     * @param height Window height
     * @param title Window title
     * @return GLFWwindow* Window instance
     *
     * @throws GladError When the initialization of GLAD fails
     */
    GLFWwindow *display_window(int width, int height, const char *title);

    /**
     * @brief Invokes the main render loop
     *
     * @param window Previously generated window
     */
    void render_loop(GLFWwindow *window) noexcept;

    ~gl_manager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    gl_manager();

    static GLFWerrorfun error_handler;
    static GLFWframebuffersizefun resize_handler;
};

} // namespace opengl_wrapper

#endif
