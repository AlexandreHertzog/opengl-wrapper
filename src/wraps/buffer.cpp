#include "buffer.h"

#include "utils/utils.h"
#include <boost/log/trivial.hpp>
#include <cassert>

namespace opengl_wrapper {

buffer::buffer(int size) : target_(0) {
    BOOST_LOG_TRIVIAL(trace) << "buffer::buffer " << *this << " size=" << size;
    assert(size > 0);
    ids_.resize(size);
    glGenBuffers(static_cast<GLsizei>(ids_.size()), ids_.data());
}

buffer::buffer(buffer &&other) noexcept : target_(0) {
    BOOST_LOG_TRIVIAL(trace) << "buffer::buffer " << *this << " other=" << &other;
    std::swap(this->ids_, other.ids_);
}

buffer::~buffer() {
    BOOST_LOG_TRIVIAL(trace) << "buffer::~buffer " << *this;
    glDeleteBuffers(static_cast<GLsizei>(ids_.size()), ids_.data());
}

buffer &buffer::operator=(buffer &&other) noexcept {
    BOOST_LOG_TRIVIAL(trace) << "buffer::operator= " << *this << " other=" << &other;
    this->ids_ = std::move(other.ids_);
    return *this;
}

void buffer::bind(int index, GLenum target) {
    BOOST_LOG_TRIVIAL(trace) << "buffer::bind " << *this << " index=" << index << ", target=" << target;
    assert(index >= 0);
    assert(index < ids_.size());

    target_ = target;
    glBindBuffer(target_, ids_[index]);
}

void buffer::load(GLsizeiptr size, const void *data, GLenum usage) { // NOLINT(*-function-const)
    BOOST_LOG_TRIVIAL(trace) << "buffer::load " << *this << " size=" << size << ", data=" << data
                             << ", usage=" << usage;
    glBufferData(target_, size, data, usage);
}

const std::vector<GLuint> &buffer::get_ids() const {
    return ids_;
}

GLenum buffer::get_target() const {
    return target_;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::buffer &b) {
    return os << "buffer" << parenthesis(&b) << " ids=" << b.get_ids() << ", target=" << b.get_target();
}

} // namespace opengl_wrapper
