#include "opengl-wrapper/textures/texture.h"

#include <cassert>

namespace opengl_wrapper {

texture::texture(GLenum target, GLuint id) : m_id(id), m_target(target) {
    graphics::instance().gl_gen_textures(1, &m_id);
}

texture::texture(opengl_wrapper::texture &&other) noexcept : m_id(other.m_id), m_target(other.m_target) {
    other.m_target = 0;
    other.m_id = 0;
}

texture::~texture() {
    if (m_id != 0) {
        graphics::instance().gl_delete_textures(1, &m_id);
    }
}

texture &texture::operator=(opengl_wrapper::texture &&other) noexcept {
    m_id = other.m_id;
    m_target = other.m_target;

    other.m_target = 0;
    other.m_id = 0;
    return *this;
}

void texture::bind() { // NOLINT(readability-make-member-function-const)
    assert(m_id != 0);
    assert(m_target != 0);
    graphics::instance().gl_bind_texture(m_target, m_id);
}

void texture::set_parameter(GLenum pname, GLint param) { // NOLINT(readability-make-member-function-const)
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
} // namespace opengl_wrapper