#include "shape.h"

#include "parsers/obj_parser.h"
#include "utils/exception.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

shape::shape(vertex_array va) : m_vertex_array(std::move(va)) {
}

shape::shape(opengl_wrapper::shape &&other) noexcept
    : m_mesh(std::move(other.m_mesh)), m_transform(std::move(other.m_transform)),
      m_material(std::move(other.m_material)), m_program(std::move(other.m_program)),
      m_textures(std::move(other.m_textures)) {
}

shape::shape(const opengl_wrapper::shape &other)
    : m_mesh(other.m_mesh), m_transform(other.m_transform), m_material(other.m_material), m_program(other.m_program),
      m_textures(other.m_textures) {
}

shape &shape::operator=(shape &&other) noexcept {
    m_mesh = std::move(other.m_mesh);
    m_transform = std::move(other.m_transform);
    m_material = std::move(other.m_material);
    m_vertex_array = std::move(other.m_vertex_array);
    m_program = std::move(other.m_program);
    m_textures = std::move(other.m_textures);
    return *this;
}

shape &shape::operator=(const shape &other) {
    if (&other != this) {
        m_mesh = other.m_mesh;
        m_transform = other.m_transform;
        m_material = other.m_material;
        m_vertex_array = vertex_array();
        m_program = other.m_program;
        m_textures = other.m_textures;
    }

    return *this;
}

std::vector<unsigned> shape::serialize_draw_order() const {
    std::vector<unsigned> ret;
    // TODO: Reenable when a way to properly serialize this is figured out.
    //    ret.reserve(m_faces.size() * m_faces[0].size());
    //    for (const auto &face : m_faces) {
    //        for (const auto &subface : face) {
    //            ret.emplace_back(subface.m_vertex_index);
    //        }
    //    }
    return ret;
}

void shape::set_program(std::shared_ptr<program> p) {
    m_program = std::move(p);
}

void shape::set_textures(textures_t t) {
    m_textures = std::move(t);
}

void shape::add_texture(opengl_wrapper::texture::pointer_t t) {
    m_textures.emplace_back(std::move(t));
}

const shape::textures_t &shape::get_textures() const {
    return m_textures;
}

void shape::load_vertices() {
    m_vertex_array.load(m_mesh.get_vertices(), serialize_draw_order(), GL_STATIC_DRAW);
}

void shape::bind() {
    assert(m_program);
    assert(!m_textures.empty());

    for (auto &t : m_textures) {
        assert(t);
        t->bind();
    }

    m_program->use(*this);
    m_vertex_array.bind();
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