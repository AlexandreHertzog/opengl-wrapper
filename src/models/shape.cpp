#include "shape.h"

#include "opengl-wrapper/textures/texture.h"

namespace opengl_wrapper {

unsigned shape::add_vertex(vertex v) {
    vertices_.emplace_back(v);
    return vertices_.size() - 1;
}

const std::vector<vertex> &shape::get_vertices() const {
    return vertices_;
}

void shape::set_draw_order(std::vector<unsigned int> indices) {
    indices_ = std::move(indices);
}

const std::vector<unsigned> &shape::get_draw_order() const {
    return indices_;
}

void shape::set_vertex_array(int va) {
    vertex_array_ = va;
}

int shape::get_vertex_array() const {
    return vertex_array_;
}

void shape::set_program(std::shared_ptr<program> p) {
    program_ = std::move(p);
}

std::shared_ptr<program> shape::get_program() const {
    return program_;
}

void shape::set_texture(std::shared_ptr<texture> t) {
    m_texture = std::move(t);
}

std::shared_ptr<texture> shape::get_texture() const {
    return m_texture;
}

} // namespace opengl_wrapper