#include "shape.h"

#include "texture.h"

namespace opengl_wrapper {

shape::shape(const shape &other)
    : m_vertices(other.m_vertices), m_indices(other.m_indices), m_program(other.m_program),
      m_textures(other.m_textures) {
}

shape::shape(opengl_wrapper::shape &&other) noexcept
    : m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
      m_vertex_array(std::move(other.m_vertex_array)), m_program(std::move(other.m_program)),
      m_textures(std::move(other.m_textures)) {
}

shape &shape::operator=(shape &&other) noexcept {
    m_vertices = std::move(other.m_vertices);
    m_indices = std::move(other.m_indices);
    m_vertex_array = std::move(other.m_vertex_array);
    m_program = std::move(other.m_program);
    m_textures = std::move(other.m_textures);
    return *this;
}

shape &shape::operator=(const shape &other) {
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;
    m_vertex_array = vertex_array();
    m_program = other.m_program;
    m_textures = other.m_textures;
    return *this;
}

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

void shape::set_vertex_array(vertex_array va) {
    m_vertex_array = std::move(va);
}

vertex_array &shape::get_vertex_array() {
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