#include "factories/texture_factory.h"

#include "data_types/image.h"

namespace game_engine {

texture_factory_t::texture_factory_t(opengl_cpp::gl_t &gl) : m_gl(gl) {
}

texture_pointer_t texture_factory_t::build_texture(const char *path, int texture_layer) {
    using opengl_cpp::texture_format_t;
    using opengl_cpp::texture_parameter_t;
    using opengl_cpp::texture_parameter_values_t;
    using opengl_cpp::texture_target_t;

    auto ret = std::make_shared<opengl_cpp::texture_t>(m_gl, texture_layer, texture_target_t::tex_2d);
    ret->bind();

    ret->set_parameter(texture_parameter_t::wrap_s, texture_parameter_values_t::repeat);
    ret->set_parameter(texture_parameter_t::wrap_t, texture_parameter_values_t::repeat);
    ret->set_parameter(texture_parameter_t::min_filter, texture_parameter_values_t::linear_mipmap_linear);
    ret->set_parameter(texture_parameter_t::mag_filter, texture_parameter_values_t::linear);

    image_t image(path);
    ret->set_image(image.get_width(), image.get_height(),
                   image.has_alpha() ? texture_format_t::rgba : texture_format_t::rgb, image.get_data());
    ret->generate_mipmap();
    return ret;
}

} // namespace game_engine