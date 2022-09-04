#include "opengl-wrapper/data_types/buffer.h"

#include "utils/utils.h"
#include <cassert>

namespace opengl_wrapper {

std::vector<buffer_t> buffer_t::build(size_t amount) {
    assert(amount > 0);

    const auto ids = graphics_t::instance().new_buffers(amount);

    std::vector<buffer_t> ret;
    ret.reserve(amount);
    for (int i = 0; i < amount; ++i) {
        ret.emplace_back(buffer_t(ids[i]));
    }
    return ret;
}

buffer_t::buffer_t(identifier_t id, buffer_target_t target) : m_target(target), m_id(id) {
    if (0 == m_id) {
        m_id = graphics_t::instance().new_buffers(1)[0];
    }
}

buffer_t::buffer_t(buffer_t &&other) noexcept : m_target(other.m_target), m_id(other.m_id) {
    other.m_target = buffer_target_t::undefined;
    other.m_id = 0;
}

buffer_t::~buffer_t() {
    if (0 != m_id) {
        graphics_t::instance().delete_buffers(1, &m_id);
    }
}

buffer_t &buffer_t::operator=(buffer_t &&other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;

    m_target = other.m_target;
    other.m_target = buffer_target_t::undefined;
    return *this;
}

void buffer_t::bind() {
    assert(0 != m_id);
    assert(buffer_target_t::undefined != m_target);
    graphics_t::instance().bind(*this);
}

identifier_t buffer_t::get_id() const {
    return m_id;
}

buffer_target_t buffer_t::get_target() const {
    return m_target;
}

void buffer_t::set_target(buffer_target_t target) {
    m_target = target;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::buffer_t &b) {
    return os << "buffer" << parenthesis(&b) << " id=" << b.get_id() << ", target=" << b.get_target();
}

} // namespace opengl_wrapper
