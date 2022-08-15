#ifndef OPENGL_WRAPPER_GL_MANAGER_H
#define OPENGL_WRAPPER_GL_MANAGER_H

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

    gl_manager(const gl_manager &) = delete;
    gl_manager(gl_manager &&) = delete;
    gl_manager &operator=(const gl_manager &) = delete;
    gl_manager &operator=(gl_manager &&) = delete;
    ~gl_manager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    gl_manager();

    static const GLFWerrorfun error_handler_;
};

} // namespace opengl_wrapper

#endif
