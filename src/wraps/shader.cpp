#include "shader.h"
#include <array>

#include "exceptions/gl_error.h"

namespace opengl_wrapper {

Shader::Shader(GLenum type, const char *source) : id_(glCreateShader(type)) {
    constexpr auto error_string_length = 512;

    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (GL_FALSE == success) {
        glGetProgramInfoLog(id_, message.size(), nullptr, message.data());
        throw GlError(message.data());
    }
}

Shader::Shader(Shader &&other) noexcept : id_(other.id_) {
    other.id_ = 0;
}

Shader::~Shader() {
    glDeleteShader(id_);
}

Shader &Shader::operator=(Shader &&other) noexcept {
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

GLuint Shader::getId() const {
    return id_;
}

} // namespace opengl_wrapper