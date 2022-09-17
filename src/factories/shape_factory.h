#pragma once

#include "data_types/shape.h"
#include "factories/texture_factory.h"
#include <memory>
#include <opengl-cpp/texture.h>

namespace game_engine {

class shape_factory_t {
  public:
    shape_factory_t(opengl_cpp::gl_t &gl, texture_factory_t &texture_factory);
    shape_pointer_t build_cube(texture_pointer_t &base_texture);
    shape_pointer_t build_plane(texture_pointer_t &base_texture);
    shape_pointer_t build_sphere(texture_pointer_t &base_texture);
    shape_pointer_t build_torus(texture_pointer_t &base_texture);
    shape_pointer_t build_light_shape(const light_pointer_t &light, texture_pointer_t &base_texture);

  private:
    opengl_cpp::gl_t &m_gl;
    texture_factory_t &m_texture_factory;
};

} // namespace game_engine
