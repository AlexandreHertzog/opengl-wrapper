#include "shape.h"

#include "glm/ext/matrix_transform.hpp"
#include "parsers/obj_parser.h"
#include "utils/exception.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

shape_t::shape_t(opengl_cpp::vertex_array_t va) : m_vertex_array(std::move(va)) {
}

shape_t::shape_t(opengl_wrapper::shape_t &&other) noexcept
    : m_mesh(std::move(other.m_mesh)), m_transform(std::move(other.m_transform)),
      m_material(std::move(other.m_material)), m_vertex_array(std::move(other.m_vertex_array)) {
}

shape_t &shape_t::operator=(shape_t &&other) noexcept {
    m_mesh = std::move(other.m_mesh);
    m_transform = std::move(other.m_transform);
    m_material = std::move(other.m_material);
    m_vertex_array = std::move(other.m_vertex_array);
    return *this;
}

void shape_t::load_vertices() {
    m_vertex_array.load(m_mesh.get_vertices());
}

void shape_t::bind() {
    if (m_material.m_texture1) {
        m_material.m_texture1->bind();
    }
    if (m_material.m_texture2) {
        m_material.m_texture2->bind();
    }
    if (m_material.m_diffuse) {
        m_material.m_diffuse->bind();
    }
    if (m_material.m_specular) {
        m_material.m_specular->bind();
    }

    m_vertex_array.bind();
}

glm::mat4 shape_t::model_transformations() const {
    auto model = glm::mat4(1.0F);
    model = glm::translate(model, m_transform.m_translation);
    model = glm::rotate(model, glm::radians(m_transform.m_rotation_angle), m_transform.m_rotation_axis);
    model = glm::scale(model, m_transform.m_scale);
    return model;
}

mesh_t &shape_t::get_mesh() {
    return m_mesh;
}

void shape_t::set_mesh(mesh_t m) {
    m_mesh = std::move(m);
}

transform_t &shape_t::get_transform() {
    return m_transform;
}

void shape_t::set_transform(transform_t t) {
    m_transform = std::move(t);
}

material_t &shape_t::get_material() {
    return m_material;
}

void shape_t::set_material(material_t m) {
    m_material = std::move(m);
}

} // namespace opengl_wrapper