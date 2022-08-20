#include "buffer.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "utils/utils.h"
#include <cassert>

namespace opengl_wrapper {

buffer::buffer(GLenum target, GLuint id) : m_target(target), m_id(id) {
    BOOST_LOG_TRIVIAL(trace) << "buffer::buffer " << *this << " target=" << m_target << " id=" << m_id;
    if (0 == m_id) {
        graphics::instance().gl_gen_buffers(1, &m_id);
    }
}

buffer::buffer(buffer &&other) noexcept : m_target(other.m_target), m_id(other.m_id) {
    BOOST_LOG_TRIVIAL(trace) << "buffer::buffer " << *this << " other=" << &other;

    other.m_target = 0;
    other.m_id = 0;
}

buffer::~buffer() {
    BOOST_LOG_TRIVIAL(trace) << "buffer::~buffer " << *this;
    if (0 != m_id) {
        graphics::instance().gl_delete_buffers(1, &m_id);
    }
}

buffer &buffer::operator=(buffer &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "buffer::operator= " << *this << " other=" << &other;
    m_id = other.m_id;
    other.m_id = 0;

    m_target = other.m_target;
    other.m_target = 0;
    return *this;
}

void buffer::bind() {
    BOOST_LOG_TRIVIAL(trace) << "buffer::bind " << *this;
    assert(0 != m_id);
    assert(0 != m_target);
    graphics::instance().gl_bind_buffer(m_target, m_id);
}

GLuint buffer::get_id() const {
    return m_id;
}

void buffer::set_id(GLuint id) {
    m_id = id;
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
