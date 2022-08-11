#include "program.h"

#include <algorithm>
#include <cassert>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Program::Program() : id_(glCreateProgram()) {
}

Program::Program(Program &&other) noexcept {
    std::swap(this->shaders_, other.shaders_);
    std::swap(this->shader_count_, other.shader_count_);
    std::swap(this->id_, other.id_);
    std::swap(this->linked_, other.linked_);
}

Program::~Program() {
    if (0 != id_) {
        glDeleteProgram(id_);
    }
}

Program &Program::operator=(Program &&other) noexcept {
    this->shaders_ = std::move(other.shaders_);

    this->shader_count_ = other.shader_count_;
    other.shader_count_ = 0;

    this->id_ = other.id_;
    other.id_ = 0;

    this->linked_ = other.linked_;
    other.linked_ = false;

    return *this;
}

void Program::addShader(Shader shader) {
    if (0 == id_) {
        id_ = glCreateProgram();
        assert(0 == shader_count_);
    }

    glAttachShader(id_, shader.getId());
    shaders_.emplace_back(std::move(shader));
    shader_count_++;
}

unsigned int Program::getShaderCount() const {
    return shader_count_;
}

void Program::link() {
    constexpr auto error_string_length = 512;

    assert(0 != id_);

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

bool Program::underConstruction() const {
    return 0 != id_ && !linked_;
}

void Program::use() { // NOLINT(readability-make-member-function-const)
    glUseProgram(id_);
}

} // namespace opengl_wrapper