#include "renderer.h"
#include "models/shape.h"
#include "opengl-wrapper/graphics/graphics.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

void renderer::add_shape(shape s) {
    shapes_.emplace_back(std::move(s));
}

void renderer::load_vertices() {
    vertex_arrays_ = std::make_unique<vertex_arrays>(shapes_.size());
    vertex_buffer_ = std::make_unique<buffer>(shapes_.size() * 2);

    for (int i = 0; i < shapes_.size(); i++) {
        vertex_arrays_->bind(i);
        vertex_buffer_->bind(2 * i, GL_ARRAY_BUFFER);
        shapes_[i].set_vertex_array(i);

        const auto &vertices = shapes_[i].get_vertices();

        vertex_buffer_->load(static_cast<GLsizeiptr>(vertices.size() * sizeof(vertex)), vertices.data(),
                             GL_STATIC_DRAW);

        const auto &indices = shapes_[i].get_draw_order();
        if (!indices.empty()) {
            vertex_buffer_->bind(2 * i + 1, GL_ELEMENT_ARRAY_BUFFER);

            vertex_buffer_->load(static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
                                 GL_STATIC_DRAW);
        }

        graphics::instance().gl_vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
        graphics::instance().gl_enable_vertex_attrib_array(0);

        graphics::instance().gl_vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                                                      reinterpret_cast<void *>(sizeof(vertex::pos_))); // NOLINT
        graphics::instance().gl_enable_vertex_attrib_array(1);
    }
}

void renderer::draw() {
    for (auto &shape : shapes_) {
        assert(shape.get_program());
        shape.get_program()->use();
        vertex_arrays_->bind(shape.get_vertex_array());

        graphics::instance().gl_draw_elements(GL_TRIANGLES, shape.get_draw_order().size(), GL_UNSIGNED_INT, nullptr);
    }
}

} // namespace opengl_wrapper
