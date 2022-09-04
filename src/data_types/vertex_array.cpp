#include "vertex_array.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "utils/utils.h"
#include <cassert>

namespace opengl_wrapper {

std::vector<vertex_array_t> vertex_array_t::build(size_t amount) {
    auto ids = graphics_t::instance().new_vertex_arrays(amount);

    std::vector<vertex_array_t> ret(amount);
    for (int i = 0; i < amount; ++i) {
        ret[i] = vertex_array_t(ids[i]);
    }
    return ret;
}

vertex_array_t::vertex_array_t(identifier_t id) : m_id(id), m_buffers(buffer_t::build(2)) {
    if (0 == m_id) {
        m_id = graphics_t::instance().new_vertex_arrays(1)[0];
    }

    assert(m_buffers.size() == 2);

    m_buffers[0].set_target(buffer_target_t::simple_array);
    m_buffers[1].set_target(buffer_target_t::element_array);
}

vertex_array_t::vertex_array_t(vertex_array_t &&other) noexcept
    : m_id(other.m_id), m_buffers(std::move(other.m_buffers)) {
    other.m_id = 0;
}

vertex_array_t::~vertex_array_t() {
    if (0 != m_id) {
        graphics_t::instance().delete_vertex_arrays(1, &m_id);
    }
}

vertex_array_t &vertex_array_t::operator=(vertex_array_t &&other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    m_buffers = std::move(other.m_buffers);
    return *this;
}

void vertex_array_t::bind() const {
    assert(0 != m_id);
    graphics_t::instance().bind(*this);
}

identifier_t vertex_array_t::get_id() const {
    return m_id;
}

void vertex_array_t::load(const std::vector<vertex_t> &vertices) {
    bind();

    m_buffers[0].bind();
    m_buffers[0].load(vertices);

    //    m_buffers[1].bind(); Disable as we don't support element drawing for now.
    //    m_buffers[1].load(indices);

    graphics_t::instance().vertex_attrib_pointer(0, 3, sizeof(vertex_t), 0);
    graphics_t::instance().enable_vertex_attrib_array(0);

    graphics_t::instance().vertex_attrib_pointer(1, 2, sizeof(vertex_t), sizeof(vertex_t::m_pos));
    graphics_t::instance().enable_vertex_attrib_array(1);

    graphics_t::instance().vertex_attrib_pointer(2, 3, sizeof(vertex_t),
                                                 sizeof(vertex_t::m_pos) + sizeof(vertex_t::m_tex));
    graphics_t::instance().enable_vertex_attrib_array(2);
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::vertex_array_t &va) {
    return os << "vertex_array" << parenthesis(&va) << " id=" << va.get_id();
}

} // namespace opengl_wrapper
