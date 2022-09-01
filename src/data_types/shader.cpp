#include "shader.h"

#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <array>
#include <cassert>
#include <fstream>

namespace opengl_wrapper {

shader::shader(shader_type_t type, const char *source) : m_id(graphics::instance().new_shader(type)) {
    if (source != nullptr) {
        compile(source, true);
    }
}

shader::shader(shader_type_t type, const std::filesystem::path &shader_path) {
    std::ifstream shader_file(shader_path);
    if (!shader_file.is_open()) {
        throw exception("shader file not found: " + shader_path.string());
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    std::string code = shader_stream.str();

    m_id = graphics::instance().new_shader(type);
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

identifier_t shader::get_id() const {
    return m_id;
}

void shader::compile(const char *source, bool free_on_error) { // NOLINT(readability-make-member-function-const)
    assert(nullptr != source);

    constexpr auto error_string_length = 512;

    graphics::instance().set_sources(*this, 1, &source);
    graphics::instance().compile(*this);

    const auto success = graphics::instance().get_parameter(*this, shader_parameter_t::compile_status);
    if (GL_FALSE == success) {
        const auto log = graphics::instance().get_info_log(*this);

        if (free_on_error) {
            gl_delete();
        }

        throw gl_error(log);
    }
}

void shader::gl_delete() {
    if (0 != m_id) {
        graphics::instance().delete_shader(m_id);
        m_id = 0;
    }
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::shader &s) {
    return os << "shader" << parenthesis(&s) << " id=" << s.get_id();
}

} // namespace opengl_wrapper
