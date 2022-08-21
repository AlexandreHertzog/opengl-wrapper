#include "shape.h"

#include "opengl-wrapper/textures/texture.h"

namespace opengl_wrapper {

unsigned shape::add_vertex(vertex v) {
    m_vertices.emplace_back(v);
    return m_vertices.size() - 1;
}

const std::vector<vertex> &shape::get_vertices() const {
    return m_vertices;
}

void shape::set_draw_order(std::vector<unsigned int> indices) {
    m_indices = std::move(indices);
}

const std::vector<unsigned> &shape::get_draw_order() const {
    return m_indices;
}

void shape::set_vertex_array(std::shared_ptr<vertex_array> va) {
    m_vertex_array = std::move(va);
}

std::shared_ptr<vertex_array> shape::get_vertex_array() const {
    return m_vertex_array;
}

void shape::set_program(std::shared_ptr<program> p) {
    m_program = std::move(p);
}

std::shared_ptr<program> shape::get_program() const {
    return m_program;
}

void shape::set_textures(textures_t t) {
    m_textures = std::move(t);
}

const shape::textures_t &shape::get_textures() const {
    return m_textures;
}

} // namespace opengl_wrapper