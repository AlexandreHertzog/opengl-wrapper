#include "buffer.h"

#include <cassert>

namespace opengl_wrapper {

Buffer::Buffer(int size) {
    assert(size > 0);
    ids_.resize(size);
    glGenBuffers(ids_.size(), ids_.data());
}

Buffer::Buffer(Buffer &&other) { std::swap(this->ids_, other.ids_); }

Buffer::~Buffer() { glDeleteBuffers(ids_.size(), ids_.data()); }

Buffer &Buffer::operator=(Buffer &&other) {
    this->ids_ = std::move(other.ids_);
    return *this;
}

void Buffer::bind(int index, GLenum target) {
    assert(index >= 0);
    assert(index < ids_.size());

    target_ = target;
    glBindBuffer(target_, ids_[index]);
}

void Buffer::buffer(GLsizeiptr size, const void *data, GLenum usage) {
    glBufferData(target_, size, data, usage);
}
} // namespace opengl_wrapper
