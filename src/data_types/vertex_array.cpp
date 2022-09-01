#include "vertex_array.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "utils/utils.h"
#include <cassert>

namespace opengl_wrapper {

std::vector<vertex_array> vertex_array::build(size_t amount) {
    auto ids = graphics::instance().gl_gen_vertex_arrays(amount);

    std::vector<vertex_array> ret(amount);
    for (int i = 0; i < amount; ++i) {
        ret[i] = vertex_array(ids[i]);
    }
    return ret;
}

vertex_array::vertex_array(identifier_t id) : m_id(id), m_buffers(buffer::build(2)) {
    if (0 == m_id) {
        m_id = graphics::instance().gl_gen_vertex_arrays(1)[0];
    }

    assert(m_buffers.size() == 2);

    m_buffers[0].set_target(buffer_target_t::simple_array);
    m_buffers[1].set_target(buffer_target_t::element_array);
}

vertex_array::vertex_array(vertex_array &&other) noexcept : m_id(other.m_id), m_buffers(std::move(other.m_buffers)) {
    other.m_id = 0;
}

vertex_array::~vertex_array() {
    if (0 != m_id) {
        graphics::instance().gl_delete_vertex_arrays(1, &m_id);
    }
}

vertex_array &vertex_array::operator=(vertex_array &&other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    m_buffers = std::move(other.m_buffers);
    return *this;
}

void vertex_array::bind() const {
    assert(0 != m_id);
    graphics::instance().gl_bind_vertex_array(m_id);
}

identifier_t vertex_array::get_id() const {
    return m_id;
}

void vertex_array::load(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices) {
    bind();

    m_buffers[0].bind();
    m_buffers[0].load(vertices);

    m_buffers[1].bind();
    m_buffers[1].load(indices);

    graphics::instance().gl_vertex_attrib_pointer(0, 3, sizeof(vertex), 0);
    graphics::instance().gl_enable_vertex_attrib_array(0);

    graphics::instance().gl_vertex_attrib_pointer(1, 2, sizeof(vertex), sizeof(vertex::m_pos));
    graphics::instance().gl_enable_vertex_attrib_array(1);

    graphics::instance().gl_vertex_attrib_pointer(2, 3, sizeof(vertex), sizeof(vertex::m_pos) + sizeof(vertex::m_tex));
    graphics::instance().gl_enable_vertex_attrib_array(2);
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::vertex_array &va) {
    return os << "vertex_array" << parenthesis(&va) << " id=" << va.get_id();
}

} // namespace opengl_wrapper
