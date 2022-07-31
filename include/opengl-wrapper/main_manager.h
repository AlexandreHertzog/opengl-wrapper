#ifndef MAIN_MANAGER_H
#define MAIN_MANAGER_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace opengl_wrapper {

class main_manager {
  public:
    /**
     * @brief Returns the global MainManager instance. Makes sure it is
     * initialized
     *
     * @return MainManager& MainManager instance
     *
     * @throws GlfwError When the initialization of GLFW fails
     */
    static main_manager &instance();

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

    ~main_manager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    main_manager();

    static GLFWerrorfun error_handler;
    static GLFWframebuffersizefun resize_handler;
};

} // namespace opengl_wrapper

#endif
