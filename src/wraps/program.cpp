#include "program.h"

#include <algorithm>
#include <cassert>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Program::Program() : shader_count_(0), id_(glCreateProgram()), linked_(false) {
}

Program::Program(Program &&other) noexcept
    : shaders_(std::move(other.shaders_)), shader_count_(other.shader_count_), id_(other.id_), linked_(other.linked_) {

    other.shader_count_ = 0;
    other.id_ = 0;
    other.linked_ = false;
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

bool Program::operator==(const opengl_wrapper::Program &other) const {
    return this->id_ == other.id_;
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

void Program::setUseCallback(opengl_wrapper::Program::UseCallback callback) {
    use_callback_ = std::move(callback);
}

int Program::getUniformLocation(const char *var_name) const {
    return glGetUniformLocation(id_, var_name);
}

void Program::use() { // NOLINT(readability-make-member-function-const)
    glUseProgram(id_);
    if (use_callback_) {
        use_callback_(*this);
    }
}

} // namespace opengl_wrapper