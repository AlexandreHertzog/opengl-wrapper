#include "vertex_arrays.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>

namespace opengl_wrapper {

vertex_arrays::vertex_arrays(int size) {
    BOOST_LOG_TRIVIAL(trace) << "vertex_arrays::vertex_arrays " << *this << " size=" << size;
    assert(size > 0);

    ids_.resize(size);
    graphics::instance().gl_gen_vertex_arrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

vertex_arrays::vertex_arrays(vertex_arrays &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "vertex_arrays::vertex_arrays " << *this << " other=" << other;
    std::swap(this->ids_, other.ids_);
}

vertex_arrays::~vertex_arrays() {
    BOOST_LOG_TRIVIAL(trace) << "vertex_arrays::~vertex_arrays " << *this;
    graphics::instance().gl_delete_vertex_arrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

vertex_arrays &vertex_arrays::operator=(vertex_arrays &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "vertex_arrays::operator= " << *this << " other=" << other;
    this->ids_ = std::move(other.ids_);
    return *this;
}

void vertex_arrays::bind(int index) {
    BOOST_LOG_TRIVIAL(trace) << "vertex_arrays::bind " << *this << " index=" << index;
    assert(index >= 0);
    assert(index < ids_.size());

    graphics::instance().gl_bind_vertex_array(ids_[index]);
}

const std::vector<GLuint> &vertex_arrays::get_ids() const {
    return ids_;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::vertex_arrays &va) {
    return os << "vertex_arrays" << parenthesis(&va) << " ids=" << va.get_ids();
}

} // namespace opengl_wrapper
