#include "renderer.h"
#include "models/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

renderer::renderer() : m_vertex_arrays(m_buffer_controller) {
}

void renderer::add_shape(shape s) {
    m_shapes.emplace_back(std::move(s));
}

void renderer::load_vertices() {
    for (auto &shape : m_shapes) {
        shape.set_vertex_array(m_vertex_arrays.add_vertex_array(0));
        shape.get_vertex_array()->load(shape.get_vertices(), shape.get_draw_order(), GL_STATIC_DRAW);
    }
}

void renderer::draw() {
    for (auto &shape : m_shapes) {
        assert(shape.get_texture());
        assert(shape.get_program());
        assert(shape.get_vertex_array());

        shape.get_texture()->bind();
        shape.get_program()->use();
        shape.get_vertex_array()->bind();

        graphics::instance().gl_draw_elements(GL_TRIANGLES, shape.get_draw_order().size(), GL_UNSIGNED_INT, nullptr);
    }
}

std::shared_ptr<texture> renderer::add_texture(const std::filesystem::path &path) {
    return m_textures.add_texture(path);
}

} // namespace opengl_wrapper
