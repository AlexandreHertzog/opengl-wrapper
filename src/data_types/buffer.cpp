#include "opengl-wrapper/data_types/buffer.h"

#include "utils/utils.h"
#include <cassert>

namespace opengl_wrapper {

std::vector<buffer> buffer::build(size_t amount) {
    assert(amount > 0);

    const auto ids = graphics::instance().gl_gen_buffers(amount);

    std::vector<buffer> ret;
    ret.reserve(amount);
    for (int i = 0; i < amount; ++i) {
        ret.emplace_back(buffer(ids[i]));
    }
    return ret;
}

buffer::buffer(GLuint id, GLenum target) : m_target(target), m_id(id) {
    if (0 == m_id) {
        m_id = graphics::instance().gl_gen_buffers(1)[0];
    }
}

buffer::buffer(buffer &&other) noexcept : m_target(other.m_target), m_id(other.m_id) {
    other.m_target = 0;
    other.m_id = 0;
}

buffer::~buffer() {
    if (0 != m_id) {
        graphics::instance().gl_delete_buffers(1, &m_id);
    }
}

buffer &buffer::operator=(buffer &&other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    m_target = other.m_target;
    other.m_target = 0;
    return *this;
}

void buffer::bind() {
    assert(0 != m_id);
    assert(0 != m_target);
    graphics::instance().gl_bind_buffer(m_target, m_id);
}

GLuint buffer::get_id() const {
    return m_id;
}

GLenum buffer::get_target() const {
    return m_target;
}

void buffer::set_target(GLenum target) {
    m_target = target;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::buffer &b) {
    return os << "buffer" << parenthesis(&b) << " id=" << b.get_id() << ", target=" << b.get_target();
}

} // namespace opengl_wrapper
