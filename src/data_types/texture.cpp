#include "opengl-wrapper/data_types/texture.h"
#include "image.h"

#include <cassert>

namespace opengl_wrapper {

texture::texture(int unit, GLenum target, GLuint id) : m_id(id), m_target(target), m_unit(unit) {
    assert(0 != target);
    assert(0 != unit);

    if (0 == m_id) {
        m_id = graphics::instance().gl_gen_textures(1)[0];
    }
}

texture::texture(opengl_wrapper::texture &&other) noexcept {
    gl_delete();

    std::swap(m_id, other.m_id);
    std::swap(m_target, other.m_target);
    std::swap(m_unit, other.m_unit);
}

texture::~texture() {
    gl_delete();
}

texture &texture::operator=(opengl_wrapper::texture &&other) noexcept {
    gl_delete();

    std::swap(m_id, other.m_id);
    std::swap(m_target, other.m_target);
    std::swap(m_unit, other.m_unit);

    return *this;
}

void texture::bind() { // NOLINT(readability-make-member-function-const)
    assert(0 != m_id);
    assert(0 != m_target);
    assert(0 != m_unit);

    graphics::instance().gl_activate_texture(m_unit);
    graphics::instance().gl_bind_texture(m_target, m_id);
}

void texture::set_parameter(uint32_t pname, GLint param) { // NOLINT(readability-make-member-function-const)
    assert(m_id != 0);
    assert(m_target != 0);

    graphics::instance().gl_tex_parameter_i(m_target, pname, param);
}

void texture::set_image( // NOLINT(readability-make-member-function-const)
    GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
    const void *data) {

    assert(m_id != 0);
    assert(m_target != 0);

    graphics::instance().gl_tex_image_2d(m_target, level, internalformat, width, height, border, format, type, data);
}

void texture::generate_mipmap() { // NOLINT(readability-make-member-function-const)
    assert(m_id != 0);
    assert(m_target != 0);
    graphics::instance().gl_generate_mipmap(m_target);
}

void texture::set_image_from_path(const std::filesystem::path &path) {
    m_target = GL_TEXTURE_2D;

    bind();

    set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image i(path);
    set_image(0, GL_RGB, i.get_width(), i.get_height(), 0, i.has_alpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
              i.get_data());
    generate_mipmap();
}

GLuint texture::get_id() const {
    return m_id;
}

GLenum texture::get_target() const {
    return m_target;
}

void texture::set_unit(int unit) {
    m_unit = unit;
}

int texture::get_unit() const {
    return m_unit;
}

void texture::gl_delete() {
    if (0 != m_id) {
        graphics::instance().gl_delete_textures(1, &m_id);

        m_id = 0;
        m_target = 0;
        m_unit = 0;
    }
}

std::ostream &operator<<(std::ostream &os, const texture &t) {
    return os << "texture " << &t << " id=" << t.get_id() << " target=" << t.get_target() << " unit=" << t.get_unit();
}

} // namespace opengl_wrapper