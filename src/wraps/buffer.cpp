#include "buffer.h"

#include <cassert>

namespace opengl_wrapper {

buffer::buffer(int size) : target_(0) {
    assert(size > 0);
    ids_.resize(size);
    glGenBuffers(static_cast<GLsizei>(ids_.size()), ids_.data());
}

buffer::buffer(buffer &&other) noexcept : target_(0) {
    std::swap(this->ids_, other.ids_);
}

buffer::~buffer() {
    glDeleteBuffers(static_cast<GLsizei>(ids_.size()), ids_.data());
}

buffer &buffer::operator=(buffer &&other) noexcept {
    this->ids_ = std::move(other.ids_);
    return *this;
}

void buffer::bind(int index, GLenum target) {
    assert(index >= 0);
    assert(index < ids_.size());

    target_ = target;
    glBindBuffer(target_, ids_[index]);
}

void buffer::load(GLsizeiptr size, const void *data, GLenum usage) { // NOLINT(*-function-const)
    glBufferData(target_, size, data, usage);
}

} // namespace opengl_wrapper
