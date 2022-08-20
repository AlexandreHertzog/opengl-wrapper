#include "buffer_controller.h"

#include <cassert>

namespace opengl_wrapper {

std::shared_ptr<buffer> &buffer_controller::operator[](int index) {
    assert(index >= 0);
    assert(index < m_buffers.size());
    return m_buffers[index];
}

std::vector<std::shared_ptr<buffer>> buffer_controller::add_buffers(GLsizei num_buffers) {
    std::vector<GLuint> ids(num_buffers);
    graphics::instance().gl_gen_buffers(num_buffers, ids.data());

    std::vector<std::shared_ptr<buffer>> ret(num_buffers);
    for (int i = 0; i < num_buffers; ++i) {
        ret[i] = std::make_shared<buffer>(ids[i]);
        m_buffers.emplace_back(ret[i]);
    }
    return ret;
}

} // namespace opengl_wrapper
