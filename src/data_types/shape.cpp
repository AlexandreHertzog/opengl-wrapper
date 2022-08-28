#include "shape.h"

#include "parsers/obj_parser.h"
#include "utils/exception.h"
#include <boost/log/trivial.hpp>

namespace opengl_wrapper {

shape shape::build_from_file(const std::filesystem::path &shape_path) {

    try {
        shape ret;

        obj_parser parser(shape_path);
        while (parser.is_good()) {
            const auto line_type = parser.line_type();
            switch (line_type) {
            case obj_parser::line_type_t::ignored:
                parser.get_line();
                break;
            case obj_parser::line_type_t::material_library:
                parser.get_line(ret.m_material_library);
                break;
            case obj_parser::line_type_t::object_name:
                parser.get_line(ret.m_name);
                break;
            case obj_parser::line_type_t::vertex:
                parser.get_line(ret.m_vertices);
                break;
            case obj_parser::line_type_t::texture_coordinate:
                parser.get_line(ret.m_texture_coords);
                break;
            case obj_parser::line_type_t::vertex_normal:
                parser.get_line(ret.m_vertex_normals);
                break;
            case obj_parser::line_type_t::used_material:
                parser.get_line(ret.m_used_material);
                break;
            case obj_parser::line_type_t::smoothing:
                parser.get_line(ret.m_smooth_shading);
                break;
            case obj_parser::line_type_t::face:
                parser.get_line(ret.m_faces);
                break;
            case obj_parser::line_type_t::undefined:
                BOOST_LOG_TRIVIAL(warning) << "Unrecognized line type for wavefront object: " << parser.get_header();
                parser.get_line();
                break;
            }
        }

        return ret;
    } catch (std::exception &e) {
        throw exception("Failed to parse wavefront object file: " + std::string(e.what()));
    }
}

shape::shape(vertex_array va) : m_vertex_array(std::move(va)) {
}

shape::shape(opengl_wrapper::shape &&other) noexcept
    : m_material_library(std::move(other.m_material_library)), m_name(std::move(other.m_name)),
      m_vertices(std::move(other.m_vertices)), m_texture_coords(std::move(other.m_texture_coords)),
      m_vertex_normals(std::move(other.m_vertex_normals)), m_used_material(std::move(other.m_used_material)),
      m_smooth_shading(other.m_smooth_shading), m_faces(std::move(other.m_faces)),
      m_vertex_array(std::move(other.m_vertex_array)), m_program(std::move(other.m_program)),
      m_textures(std::move(other.m_textures)), m_translation(std::move(other.m_translation)),
      m_rotation_angle(other.m_rotation_angle), m_rotation_axis(std::move(other.m_rotation_axis)),
      m_scale(std::move(other.m_scale)) {

    other.m_smooth_shading = false;
    other.m_rotation_angle = 0.0F;
}

shape::shape(const opengl_wrapper::shape &other)
    : m_material_library(other.m_material_library), m_name(other.m_name), m_vertices(other.m_vertices),
      m_texture_coords(other.m_texture_coords), m_vertex_normals(other.m_vertex_normals),
      m_used_material(other.m_used_material), m_smooth_shading(other.m_smooth_shading), m_faces(other.m_faces),
      m_vertex_array(), m_program(other.m_program), m_textures(other.m_textures), m_translation(other.m_translation),
      m_rotation_angle(other.m_rotation_angle), m_rotation_axis(other.m_rotation_axis), m_scale(other.m_scale) {
}

shape &shape::operator=(shape &&other) noexcept {
    m_material_library = std::move(other.m_material_library);
    m_name = std::move(other.m_name);
    m_vertices = std::move(other.m_vertices);
    m_texture_coords = std::move(other.m_texture_coords);
    m_vertex_normals = std::move(other.m_vertex_normals);
    m_used_material = std::move(other.m_used_material);
    m_smooth_shading = other.m_smooth_shading;
    other.m_smooth_shading = false;
    m_faces = std::move(other.m_faces);
    m_vertex_array = std::move(other.m_vertex_array);
    m_program = std::move(other.m_program);
    m_textures = std::move(other.m_textures);
    m_translation = std::move(other.m_translation);
    m_rotation_angle = other.m_rotation_angle;
    other.m_rotation_angle = 0.0F;
    m_rotation_axis = std::move(other.m_rotation_axis);
    m_scale = std::move(other.m_scale);
    return *this;
}

shape &shape::operator=(const shape &other) {
    if (&other != this) {
        m_material_library = other.m_material_library;
        m_name = other.m_name;
        m_vertices = other.m_vertices;
        m_texture_coords = other.m_texture_coords;
        m_vertex_normals = other.m_vertex_normals;
        m_used_material = other.m_used_material;
        m_smooth_shading = other.m_smooth_shading;
        m_faces = other.m_faces;
        m_vertex_array = vertex_array();
        m_program = other.m_program;
        m_textures = other.m_textures;
        m_translation = other.m_translation;
        m_rotation_angle = other.m_rotation_angle;
        m_rotation_axis = other.m_rotation_axis;
        m_scale = other.m_scale;
    }

    return *this;
}

std::vector<vertex> shape::serialize_vertices() const {
    std::vector<vertex> ret;
    ret.reserve(m_faces.size() * 3);
    for (const auto &face : m_faces) {
        assert(3 == face.size());
        vertex v1{m_vertices[face[0].m_vertex_index - 1], {}, {m_texture_coords[face[0].m_texture_coord_index - 1]}};
        vertex v2{m_vertices[face[1].m_vertex_index - 1], {}, {m_texture_coords[face[1].m_texture_coord_index - 1]}};
        vertex v3{m_vertices[face[2].m_vertex_index - 1], {}, {m_texture_coords[face[2].m_texture_coord_index - 1]}};
        ret.emplace_back(v1);
        ret.emplace_back(v2);
        ret.emplace_back(v3);
    }
    return ret;
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
    m_vertex_array.load(serialize_vertices(), serialize_draw_order(), GL_STATIC_DRAW);
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

void shape::set_translation(glm::vec3 translation) {
    m_translation = std::move(translation);
}

const glm::vec3 &shape::get_translation() const {
    return m_translation;
}

void shape::set_rotation(float angle, glm::vec3 axis) {
    m_rotation_angle = angle;
    m_rotation_axis = std::move(axis);
}

float shape::get_rotation_angle() const {
    return m_rotation_angle;
}

const glm::vec3 &shape::get_rotation_axis() const {
    return m_rotation_axis;
}

void shape::set_scale(glm::vec3 scale) {
    m_scale = std::move(scale);
}

const glm::vec3 &shape::get_scale() const {
    return m_scale;
}

} // namespace opengl_wrapper