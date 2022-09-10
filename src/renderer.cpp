#include "renderer.h"

#include "data_types/shape.h"

namespace game_engine {

renderer_t::renderer_t(opengl_cpp::gl_t &gl) : m_gl(gl) {
}

void renderer_t::draw(shape_t &s) {
    s.bind();
    m_gl.draw_arrays(0, s.get_mesh().get_vertices().size());
}

void renderer_t::set_viewport(size_t width, size_t height) {
    m_gl.set_viewport(width, height);
}

void renderer_t::set_wireframe_mode(bool enable) {
    if (enable) {
        m_gl.polygon_mode(opengl_cpp::polygon_mode_t::line);
    } else {
        m_gl.polygon_mode(opengl_cpp::polygon_mode_t::fill);
    }
}

void renderer_t::set_depth_test(bool enable) {
    if (enable) {
        m_gl.enable(opengl_cpp::graphics_feature_t::depth_test);
    } else {
        m_gl.disable(opengl_cpp::graphics_feature_t::depth_test);
    }
}

void renderer_t::set_clear_color(const glm::vec4 &c) {
    m_gl.set_clear_color(c);
}

void renderer_t::clear() {
    m_gl.clear();
}

} // namespace game_engine
