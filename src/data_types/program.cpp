#include "opengl-wrapper/data_types/program.h"

#include "opengl-wrapper/data_types/shader.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/utils/gl_error.h"
#include "utils/utils.h"
#include <algorithm>
#include <cassert>

namespace opengl_wrapper {

program_t::program_t() : m_shader_count(0), m_id(graphics_t::instance().new_program()), m_linked(false) {
}

program_t::program_t(program_t &&other) noexcept
    : m_shaders(std::move(other.m_shaders)), m_shader_count(other.m_shader_count), m_id(other.m_id),
      m_linked(other.m_linked) {

    other.m_shader_count = 0;
    other.m_id = 0;
    other.m_linked = false;
}

program_t::~program_t() {
    if (0 != m_id) {
        graphics_t::instance().delete_program(m_id);
    }
}

program_t &program_t::operator=(program_t &&other) noexcept {
    this->m_shaders = std::move(other.m_shaders);

    this->m_shader_count = other.m_shader_count;
    other.m_shader_count = 0;

    this->m_id = other.m_id;
    other.m_id = 0;

    this->m_linked = other.m_linked;
    other.m_linked = false;

    return *this;
}

bool program_t::operator==(const opengl_wrapper::program_t &other) const {
    return this->m_id == other.m_id;
}

void program_t::add_shader(shader_t shader) {
    if (0 == m_id) {
        m_id = graphics_t::instance().new_program();
        assert(0 == m_shader_count);
    }

    graphics_t::instance().attach_shader(*this, shader);
    m_shaders.emplace_back(std::move(shader));
    m_shader_count++;
}

void program_t::link() {
    assert(0 != m_id);

    graphics_t::instance().link(*this);

    const auto success = graphics_t::instance().get_parameter(*this, program_parameter_t::link_status);
    if (GL_FALSE == success) {
        throw gl_error_t(graphics_t::instance().get_info_log(*this));
    }

    m_shaders.clear();
    m_linked = true;
}

int program_t::get_uniform_location(const char *var_name) const {
    return graphics_t::instance().get_uniform_location(*this, var_name);
}

void program_t::use() const {
    graphics_t::instance().use(*this);
}

const std::vector<shader_t> &program_t::get_shaders() const {
    return m_shaders;
}

identifier_t program_t::get_id() const {
    return m_id;
}

bool program_t::get_linked() const {
    return m_linked;
}

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::program_t &p) {
    return os << "program" << parenthesis(&p) << " shaders=" << p.get_shaders() << ", id=" << p.get_id()
              << ", linked=" << p.get_linked();
}

} // namespace opengl_wrapper
