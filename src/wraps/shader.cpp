#include "shader.h"
#include <array>
#include <cassert>

#include "exceptions/gl_error.h"
#include <fstream>

namespace opengl_wrapper {

Shader::Shader(GLenum type, const char *source) : id_(glCreateShader(type)) {
    if (source != nullptr) {
        compile(source);
    }
}

Shader::Shader(GLenum type, const std::filesystem::path &shader_path) : id_(0) {
    std::ifstream shader_file(shader_path);
    if (!shader_file.is_open()) {
        throw Exception("Shader file not found: " + shader_path.string());
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    std::string code = shader_stream.str();

    id_ = glCreateShader(type);
    compile(code.c_str());
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

void Shader::compile(const char *source) { // NOLINT(readability-make-member-function-const)
    assert(nullptr != source);

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

} // namespace opengl_wrapper