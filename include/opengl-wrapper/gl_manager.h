#pragma once

#include "opengl-wrapper/graphics/graphics.h"
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

    static const GLFWerrorfun m_error_handler;
};

} // namespace opengl_wrapper
