#ifndef OPENGL_WRAPPER_VERTEX_ARRAY_CONTROLLER_H
#define OPENGL_WRAPPER_VERTEX_ARRAY_CONTROLLER_H

#include "vertex_array.h"

namespace opengl_wrapper {

class buffer_controller;

class vertex_array_controller {
  public:
    explicit vertex_array_controller(buffer_controller &bc);
    std::shared_ptr<vertex_array> &operator[](int index);
    std::shared_ptr<vertex_array> add_vertex_array(GLuint id);

  private:
    buffer_controller &m_buffer_controller;
    std::vector<std::shared_ptr<vertex_array>> m_vertex_arrays;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_VERTEX_ARRAY_CONTROLLER_H
