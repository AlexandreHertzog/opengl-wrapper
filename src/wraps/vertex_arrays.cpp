#include "vertex_arrays.h"

#include <cassert>

namespace opengl_wrapper {

VertexArrays::VertexArrays(int size) {
    assert(size > 0);

    ids_.resize(size);
    glGenVertexArrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

VertexArrays::VertexArrays(VertexArrays &&other) noexcept {
    std::swap(this->ids_, other.ids_);
}

VertexArrays::~VertexArrays() {
    glDeleteVertexArrays(static_cast<GLsizei>(ids_.size()), ids_.data());
}

VertexArrays &VertexArrays::operator=(VertexArrays &&other) noexcept {
    this->ids_ = std::move(other.ids_);
    return *this;
}

void VertexArrays::bind(int index) {
    assert(index >= 0);
    assert(index < ids_.size());

    glBindVertexArray(ids_[index]);
}

} // namespace opengl_wrapper
