#include "vertex_arrays.h"

#include <cassert>

namespace opengl_wrapper {

vertex_arrays::vertex_arrays(int size) {
    assert(size > 0);

    ids_.resize(size);
    glGenVertexArrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

vertex_arrays::vertex_arrays(vertex_arrays &&other) noexcept {
    std::swap(this->ids_, other.ids_);
}

vertex_arrays::~vertex_arrays() {
    glDeleteVertexArrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

vertex_arrays &vertex_arrays::operator=(vertex_arrays &&other) noexcept {
    this->ids_ = std::move(other.ids_);
    return *this;
}

void vertex_arrays::bind(int index) {
    assert(index >= 0);
    assert(index < ids_.size());

    glBindVertexArray(ids_[index]);
}

} // namespace opengl_wrapper
