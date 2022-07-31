#ifndef gl_manager_H
#define gl_manager_H

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

    ~GlManager();

  private:
    /**
     * @brief Construct a new Main Manager object
     * @throws GlfwError When the initialization of GLFW fails
     *
     */
    GlManager();

    static GLFWerrorfun error_handler_;
};

} // namespace opengl_wrapper

#endif
