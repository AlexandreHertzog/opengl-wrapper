#include "vertex_array_controller.h"

#include "buffer_controller.h"

namespace opengl_wrapper {

vertex_array_controller::vertex_array_controller(buffer_controller &bc) : m_buffer_controller(bc) {
}

std::shared_ptr<vertex_array> &vertex_array_controller::operator[](int index) {
    return m_vertex_arrays.at(index);
}

std::shared_ptr<vertex_array> vertex_array_controller::add_vertex_array(GLuint id) {
    return m_vertex_arrays.emplace_back(std::make_shared<vertex_array>(m_buffer_controller.add_buffers(2), id));
}

} // namespace opengl_wrapper
