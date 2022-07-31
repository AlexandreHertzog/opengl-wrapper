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

    ~gl_manager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    gl_manager();

    static GLFWerrorfun error_handler;
};

} // namespace opengl_wrapper

#endif
