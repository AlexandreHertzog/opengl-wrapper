#include "mesh.h"

#include "parsers/obj_parser.h"
#include <boost/log/trivial.hpp>

namespace game_engine {

mesh_t::mesh_t(const std::filesystem::path &wavefront_object_path) {

    try {
        obj_parser_t parser(wavefront_object_path);
        while (parser.is_good()) {
            const auto line_type = parser.line_type();
            switch (line_type) {
            case obj_parser_t::line_type_t::ignored:
                parser.get_line();
                break;
            case obj_parser_t::line_type_t::material_library:
                parser.get_line(m_material_library);
                break;
            case obj_parser_t::line_type_t::object_name:
                parser.get_line(m_name);
                break;
            case obj_parser_t::line_type_t::vertex:
                parser.get_line(m_vertices);
                break;
            case obj_parser_t::line_type_t::texture_coordinate:
                parser.get_line(m_texture_coords);
                break;
            case obj_parser_t::line_type_t::vertex_normal:
                parser.get_line(m_vertex_normals);
                break;
            case obj_parser_t::line_type_t::used_material:
                parser.get_line(m_used_material);
                break;
            case obj_parser_t::line_type_t::smoothing:
                parser.get_line(m_smooth_shading);
                break;
            case obj_parser_t::line_type_t::face:
                parser.get_line(m_faces);
                break;
            case obj_parser_t::line_type_t::undefined:
                BOOST_LOG_TRIVIAL(warning) << "Unrecognized line type for wavefront object: " << parser.get_header();
                parser.get_line();
                break;
            }
        }
        cache_vertices();
    } catch (std::exception &e) {
        throw exception_t("Failed to parse wavefront object file: " + std::string(e.what()));
    }
}

mesh_t::mesh_t(game_engine::mesh_t &&other) noexcept
    : m_material_library(std::move(other.m_material_library)), m_name(std::move(other.m_name)),
      m_vertices(std::move(other.m_vertices)), m_texture_coords(std::move(other.m_texture_coords)),
      m_vertex_normals(std::move(other.m_vertex_normals)), m_used_material(std::move(other.m_used_material)),
      m_smooth_shading(other.m_smooth_shading), m_faces(std::move(other.m_faces)),
      m_cached_vertices(std::move(other.m_cached_vertices)) {

    other.m_smooth_shading = false;
}

mesh_t &mesh_t::operator=(game_engine::mesh_t &&other) noexcept {
    std::swap(m_material_library, other.m_material_library);
    std::swap(m_name, other.m_name);
    std::swap(m_vertices, other.m_vertices);
    std::swap(m_texture_coords, other.m_texture_coords);
    std::swap(m_vertex_normals, other.m_vertex_normals);
    std::swap(m_used_material, other.m_used_material);
    std::swap(m_smooth_shading, other.m_smooth_shading);
    std::swap(m_faces, other.m_faces);
    std::swap(m_cached_vertices, other.m_cached_vertices);
    return *this;
}

mesh_t &mesh_t::operator=(const game_engine::mesh_t &other) {
    if (this != &other) {
        m_material_library = other.m_material_library;
        m_name = other.m_name;
        m_vertices = other.m_vertices;
        m_texture_coords = other.m_texture_coords;
        m_vertex_normals = other.m_vertex_normals;
        m_used_material = other.m_used_material;
        m_smooth_shading = other.m_smooth_shading;
        m_faces = other.m_faces;
        m_cached_vertices = other.m_cached_vertices;
    }
    return *this;
}

const std::vector<opengl_cpp::vertex_t> &mesh_t::get_vertices() const {
    return m_cached_vertices;
}

const std::string &mesh_t::get_name() const {
    return m_name;
}

void mesh_t::cache_vertices() {
    m_cached_vertices.clear();
    m_cached_vertices.reserve(m_faces.size() * 3);
    for (const auto &face : m_faces) {
        assert(3 == face.size());
        opengl_cpp::vertex_t v1{m_vertices[face[0].m_vertex_index - 1],
                                {m_texture_coords[face[0].m_texture_coord_index - 1]},
                                {m_vertex_normals[face[0].m_normal_index - 1]}};
        opengl_cpp::vertex_t v2{m_vertices[face[1].m_vertex_index - 1],
                                {m_texture_coords[face[1].m_texture_coord_index - 1]},
                                {m_vertex_normals[face[1].m_normal_index - 1]}};
        opengl_cpp::vertex_t v3{m_vertices[face[2].m_vertex_index - 1],
                                {m_texture_coords[face[2].m_texture_coord_index - 1]},
                                {m_vertex_normals[face[2].m_normal_index - 1]}};
        m_cached_vertices.emplace_back(v1);
        m_cached_vertices.emplace_back(v2);
        m_cached_vertices.emplace_back(v3);
    }
}

} // namespace game_engine