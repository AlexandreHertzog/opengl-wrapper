#ifndef OPENGL_WRAPPER_GL_MANAGER_H
#define OPENGL_WRAPPER_GL_MANAGER_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace opengl_wrapper {

class GlManager {
  public:
    /**
     * @brief Returns the global MainManager instance. Makes sure it is
     * initialized
     *
     * @return MainManager& MainManager instance
     *
     * @throws GlfwError When the initialization of GLFW fails
     */
    static GlManager &instance();

    GlManager(const GlManager &) = delete;
    GlManager(GlManager &&) = delete;
    GlManager &operator=(const GlManager &) = delete;
    GlManager &operator=(GlManager &&) = delete;
    ~GlManager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    GlManager();

    static const GLFWerrorfun error_handler_;
};

} // namespace opengl_wrapper

#endif
