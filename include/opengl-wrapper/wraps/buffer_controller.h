#ifndef OPENGL_WRAPPER_BUFFER_CONTROLLER_H
#define OPENGL_WRAPPER_BUFFER_CONTROLLER_H

#include "buffer.h"

#include <memory>

namespace opengl_wrapper {

class buffer_controller {
  public:
    /**
     * @brief Gets the buffer at the index.
     * @param index Buffer index.
     * @return Buffer.
     */
    std::shared_ptr<buffer> &operator[](int index);

    /**
     * Creates many buffers. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
     * @param num_buffers
     * @return
     */
    std::vector<std::shared_ptr<buffer>> add_buffers(GLsizei num_buffers);

  private:
    std::vector<std::shared_ptr<buffer>> m_buffers;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_BUFFER_CONTROLLER_H
