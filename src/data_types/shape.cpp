#include "shape.h"

#include "glm/ext/matrix_transform.hpp"
#include "parsers/obj_parser.h"
#include "utils/exception.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

shape::shape(vertex_array va) : m_vertex_array(std::move(va)) {
}

shape::shape(opengl_wrapper::shape &&other) noexcept
    : m_mesh(std::move(other.m_mesh)), m_transform(std::move(other.m_transform)),
      m_material(std::move(other.m_material)) {
}

shape::shape(const opengl_wrapper::shape &other)
    : m_mesh(other.m_mesh), m_transform(other.m_transform), m_material(other.m_material) {
}

shape &shape::operator=(shape &&other) noexcept {
    m_mesh = std::move(other.m_mesh);
    m_transform = std::move(other.m_transform);
    m_material = std::move(other.m_material);
    m_vertex_array = std::move(other.m_vertex_array);
    return *this;
}

shape &shape::operator=(const shape &other) {
    if (&other != this) {
        m_mesh = other.m_mesh;
        m_transform = other.m_transform;
        m_material = other.m_material;
        m_vertex_array = vertex_array();
    }

    return *this;
}

void shape::load_vertices() {
    m_vertex_array.load(m_mesh.get_vertices());
}

void shape::bind() {
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

glm::mat4 shape::model_transformations() const {
    auto model = glm::mat4(1.0F);
    model = glm::translate(model, m_transform.m_translation);
    model = glm::rotate(model, glm::radians(m_transform.m_rotation_angle), m_transform.m_rotation_axis);
    model = glm::scale(model, m_transform.m_scale);
    return model;
}

mesh &shape::get_mesh() {
    return m_mesh;
}

void shape::set_mesh(mesh m) {
    m_mesh = std::move(m);
}

transform &shape::get_transform() {
    return m_transform;
}

void shape::set_transform(transform t) {
    m_transform = std::move(t);
}

material &shape::get_material() {
    return m_material;
}

void shape::set_material(material m) {
    m_material = std::move(m);
}

} // namespace opengl_wrapper