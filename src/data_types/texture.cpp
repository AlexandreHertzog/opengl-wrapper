#include "texture.h"

#include "image.h"

namespace opengl_wrapper {

texture_t::texture_t(int unit, texture_target_t target, identifier_t id) : m_id(id), m_target(target), m_unit(unit) {
    assert(0 <= unit);

    if (0 == m_id) {
        m_id = graphics_t::instance().new_textures(1)[0];
    }
}

texture_t::texture_t(opengl_wrapper::texture_t &&other) noexcept {
    gl_delete();

    std::swap(m_id, other.m_id);
    std::swap(m_target, other.m_target);
    std::swap(m_unit, other.m_unit);
}

texture_t::~texture_t() {
    gl_delete();
}

texture_t &texture_t::operator=(opengl_wrapper::texture_t &&other) noexcept {
    gl_delete();

    std::swap(m_id, other.m_id);
    std::swap(m_target, other.m_target);
    std::swap(m_unit, other.m_unit);

    return *this;
}

texture_t::pointer_t texture_t::build(const std::filesystem::path &path, int unit) {
    auto ret = std::make_shared<texture_t>(unit);
    ret->set_image_from_path(path);
    return ret;
}

void texture_t::bind() {
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);
    assert(0 <= m_unit);

    graphics_t::instance().activate(*this);
    graphics_t::instance().bind(*this);
}

void texture_t::set_image(size_t width, size_t height, texture_format_t format, const unsigned char *data) {
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);

    graphics_t::instance().set_image(width, height, format, data);
}

void texture_t::generate_mipmap() {
    assert(0 != m_id);
    assert(texture_target_t::undefined != m_target);
    graphics_t::instance().generate_mipmap(*this);
}

void texture_t::set_image_from_path(const std::filesystem::path &path) {
    m_target = texture_target_t::tex_2d;

    bind();

    set_parameter(texture_parameter_t::wrap_s, texture_parameter_values_t::repeat);
    set_parameter(texture_parameter_t::wrap_t, texture_parameter_values_t::repeat);
    set_parameter(texture_parameter_t::min_filter, texture_parameter_values_t::linear_mipmap_linear);
    set_parameter(texture_parameter_t::mag_filter, texture_parameter_values_t::linear);

    image_t image(path);
    set_image(image.get_width(), image.get_height(), image.has_alpha() ? texture_format_t::rgba : texture_format_t::rgb,
              image.get_data());
    generate_mipmap();
}

identifier_t texture_t::get_id() const {
    return m_id;
}

texture_target_t texture_t::get_target() const {
    return m_target;
}

int texture_t::get_unit() const {
    return m_unit;
}

void texture_t::gl_delete() {
    if (0 != m_id) {
        graphics_t::instance().delete_textures(1, &m_id);

        m_id = 0;
        m_target = texture_target_t::undefined;
        m_unit = -1;
    }
}

std::ostream &operator<<(std::ostream &os, const texture_t &t) {
    return os << "texture " << &t << " id=" << t.get_id() << " target=" << t.get_target() << " unit=" << t.get_unit();
}

} // namespace opengl_wrapper