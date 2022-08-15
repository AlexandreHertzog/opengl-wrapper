#include "shader.h"
#include <array>
#include <cassert>

#include "exceptions/gl_error.h"
#include <fstream>

namespace opengl_wrapper {

shader::shader(GLenum type, const char *source) : id_(glCreateShader(type)) {
    if (source != nullptr) {
        compile(source);
    }
}

shader::shader(GLenum type, const std::filesystem::path &shader_path) : id_(0) {
    std::ifstream shader_file(shader_path);
    if (!shader_file.is_open()) {
        throw exception("shader file not found: " + shader_path.string());
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    std::string code = shader_stream.str();

    id_ = glCreateShader(type);
    compile(code.c_str());
}

shader::shader(shader &&other) noexcept : id_(other.id_) {
    other.id_ = 0;
}

shader::~shader() {
    glDeleteShader(id_);
}

shader &shader::operator=(shader &&other) noexcept {
    this->id_ = other.id_;
    other.id_ = 0;
    return *this;
}

GLuint shader::get_id() const {
    return id_;
}

void shader::compile(const char *source) { // NOLINT(readability-make-member-function-const)
    assert(nullptr != source);

    constexpr auto error_string_length = 512;

    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (GL_FALSE == success) {
        glGetShaderInfoLog(id_, message.size(), nullptr, message.data());
        throw gl_error(message.data());
    }
}

} // namespace opengl_wrapper