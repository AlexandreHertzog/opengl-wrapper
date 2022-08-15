#include "program.h"

#include <algorithm>
#include <cassert>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

program::program() : shader_count_(0), id_(glCreateProgram()), linked_(false) {
}

program::program(program &&other) noexcept
    : shaders_(std::move(other.shaders_)), shader_count_(other.shader_count_), id_(other.id_), linked_(other.linked_) {

    other.shader_count_ = 0;
    other.id_ = 0;
    other.linked_ = false;
}

program::~program() {
    if (0 != id_) {
        glDeleteProgram(id_);
    }
}

program &program::operator=(program &&other) noexcept {
    this->shaders_ = std::move(other.shaders_);

    this->shader_count_ = other.shader_count_;
    other.shader_count_ = 0;

    this->id_ = other.id_;
    other.id_ = 0;

    this->linked_ = other.linked_;
    other.linked_ = false;

    return *this;
}

bool program::operator==(const opengl_wrapper::program &other) const {
    return this->id_ == other.id_;
}

void program::add_shader(shader shader) {
    if (0 == id_) {
        id_ = glCreateProgram();
        assert(0 == shader_count_);
    }

    glAttachShader(id_, shader.get_id());
    shaders_.emplace_back(std::move(shader));
    shader_count_++;
}

void program::link() {
    constexpr auto error_string_length = 512;

    assert(0 != id_);

    glLinkProgram(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (GL_FALSE == success) {
        glGetProgramInfoLog(id_, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }

    shaders_.clear();
}

void program::set_use_callback(opengl_wrapper::program::usa_callback callback) {
    use_callback_ = std::move(callback);
}

int program::get_uniform_location(const char *var_name) const {
    return glGetUniformLocation(id_, var_name);
}

void program::use() { // NOLINT(readability-make-member-function-const)
    glUseProgram(id_);
    if (use_callback_) {
        use_callback_(*this);
    }
}

} // namespace opengl_wrapper