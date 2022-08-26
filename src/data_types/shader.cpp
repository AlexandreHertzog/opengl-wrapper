#include "shader.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <array>
#include <boost/log/trivial.hpp>
#include <cassert>
#include <fstream>

namespace opengl_wrapper {

shader::shader(GLenum type, const char *source) : m_id(graphics::instance().gl_create_shader(type)) {
    if (source != nullptr) {
        compile(source, true);
    }
}

shader::shader(GLenum type, const std::filesystem::path &shader_path) {
    std::ifstream shader_file(shader_path);
    if (!shader_file.is_open()) {
        throw exception("shader file not found: " + shader_path.string());
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    std::string code = shader_stream.str();

    m_id = graphics::instance().gl_create_shader(type);
    compile(code.c_str(), false);
}

shader::shader(shader &&other) noexcept {
    gl_delete();
    std::swap(m_id, other.m_id);
}

shader::~shader() {
    gl_delete();
}

shader &shader::operator=(shader &&other) noexcept {
    gl_delete();
    std::swap(m_id, other.m_id);
    return *this;
}

GLuint shader::get_id() const {
    return m_id;
}

void shader::compile(const char *source, bool free_on_error) { // NOLINT(readability-make-member-function-const)
    assert(nullptr != source);

    constexpr auto error_string_length = 512;

    graphics::instance().gl_shader_source(m_id, 1, &source, nullptr);
    graphics::instance().gl_compile_shader(m_id);

    int success = 0;
    std::array<char, error_string_length> message = {'\0'};

    graphics::instance().gl_get_shaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (GL_FALSE == success) {
        graphics::instance().gl_get_shader_info_log(m_id, message.size(), nullptr, message.data());

        if (free_on_error) {
            gl_delete();
        }

        throw gl_error(message.data());
    }
}

void shader::gl_delete() {
    if (0 != m_id) {
        graphics::instance().gl_delete_shader(m_id);
        m_id = 0;
    }
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::shader &s) {
    return os << "shader" << parenthesis(&s) << " id=" << s.get_id();
}

} // namespace opengl_wrapper
