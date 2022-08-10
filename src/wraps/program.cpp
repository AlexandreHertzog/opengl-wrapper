#include "program.h"

#include <algorithm>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Program::Program() : id_(glCreateProgram()) {
}

Program::Program(Program &&other) noexcept {
    std::swap(this->id_, other.id_);
}

Program::~Program() {
    glDeleteProgram(id_);
}

Program &Program::operator=(Program &&other) noexcept {
    this->id_ = other.id_;
    other.id_ = 0;

    this->shaders_ = std::move(other.shaders_);
    return *this;
}

void Program::addShader(Shader shader) {
    glAttachShader(id_, shader.getId());
    shaders_.emplace_back(std::move(shader));
}

void Program::link() {
    constexpr auto error_string_length = 512;

    glLinkProgram(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (GL_FALSE == success) {
        glGetProgramInfoLog(id_, message.size(), nullptr, message.data());
        throw GlError(message.data());
    }

    shaders_.clear();
}

void Program::use() { // NOLINT(readability-make-member-function-const)
    glUseProgram(id_);
}

} // namespace opengl_wrapper