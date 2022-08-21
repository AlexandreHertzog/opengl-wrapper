#include "renderer.h"
#include "opengl-wrapper/data_types/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

void renderer::add_shape(shape s) {
    m_shapes.emplace_back(std::move(s));
}

void renderer::load_vertices() {
    for (auto &shape : m_shapes) {
        shape.get_vertex_array().load(shape.get_vertices(), shape.get_draw_order(), GL_STATIC_DRAW);
    }
}

void renderer::draw() {
    for (auto &shape : m_shapes) {
        assert(!shape.get_textures().empty());
        assert(shape.get_program());

        for (auto &t : shape.get_textures()) {
            assert(t);
            t->bind();
        }

        shape.get_program()->use();
        shape.get_vertex_array().bind();

        if (!shape.get_draw_order().empty()) {
            graphics::instance().gl_draw_elements(GL_TRIANGLES, shape.get_draw_order().size(), GL_UNSIGNED_INT,
                                                  nullptr);
        } else {
            graphics::instance().gl_draw_arrays(GL_TRIANGLES, 0, shape.get_vertices().size());
        }
    }
}

std::shared_ptr<texture> renderer::add_texture(const std::filesystem::path &path, int unit) {
    return m_textures.add_texture(path, unit);
}

} // namespace opengl_wrapper
