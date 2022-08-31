#include "texture.h"

#include "image.h"

namespace opengl_wrapper {

texture::texture(int unit, texture_target_t target, GLuint id) : m_id(id), m_target(target), m_unit(unit) {
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

texture::pointer_t texture::build(const std::filesystem::path &path, int unit) {
    auto ret = std::make_shared<texture>(unit);
    ret->set_image_from_path(path);
    return ret;
}

void texture::bind() { // NOLINT(readability-make-member-function-const)
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);
    assert(0 != m_unit);

    graphics::instance().gl_activate_texture(m_unit);
    graphics::instance().gl_bind_texture(m_target, m_id);
}

void texture::set_image(size_t width, size_t height, texture_format_t format, const unsigned char *data) {
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);

    graphics::instance().gl_tex_image_2d(m_target, width, height, format, data);
}

void texture::generate_mipmap() { // NOLINT(readability-make-member-function-const)
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);
    graphics::instance().gl_generate_mipmap(m_target);
}

void texture::set_image_from_path(const std::filesystem::path &path) {
    m_target = texture_target_t::tex_2d;

    bind();

    set_parameter(texture_parameter_t::wrap_s, texture_parameter_values_t::repeat);
    set_parameter(texture_parameter_t::wrap_t, texture_parameter_values_t::repeat);
    set_parameter(texture_parameter_t::min_filter, texture_parameter_values_t::linear_mipmap_linear);
    set_parameter(texture_parameter_t::mag_filter, texture_parameter_values_t::linear);

    image i(path);
    set_image(i.get_width(), i.get_height(), i.has_alpha() ? texture_format_t::rgba : texture_format_t::rgb,
              i.get_data());
    generate_mipmap();
}

GLuint texture::get_id() const {
    return m_id;
}

texture_target_t texture::get_target() const {
    return m_target;
}

int texture::get_unit() const {
    return m_unit;
}

void texture::gl_delete() {
    if (0 != m_id) {
        graphics::instance().gl_delete_textures(1, &m_id);

        m_id = 0;
        m_target = texture_target_t::undefined;
        m_unit = 0;
    }
}

std::ostream &operator<<(std::ostream &os, const texture &t) {
    return os << "texture " << &t << " id=" << t.get_id() << " target=" << t.get_target() << " unit=" << t.get_unit();
}

} // namespace opengl_wrapper