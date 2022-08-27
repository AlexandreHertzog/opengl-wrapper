#pragma once

#include "face_vertex_indices.h"
#include "vertex.h"
#include "vertex_array.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>

namespace opengl_wrapper {

class program;
class texture;

class shape {
  public:
    using texture_pointer_t = std::shared_ptr<texture>;
    using textures_t = std::vector<texture_pointer_t>;

    static shape build_from_file(const std::filesystem::path &shape_path);

    explicit shape(vertex_array va = vertex_array());
    shape(const shape &other);
    shape(shape &&other) noexcept;
    ~shape() = default;

    shape &operator=(const shape &other);
    shape &operator=(shape &&other) noexcept;

    [[nodiscard]] std::vector<vertex> serialize_vertices() const;
    [[nodiscard]] std::vector<unsigned> serialize_draw_order() const;
    [[nodiscard]] const std::vector<glm::vec2> &get_texture_coords() const;
    [[nodiscard]] vertex_array &get_vertex_array();

    void set_program(std::shared_ptr<program> p);
    [[nodiscard]] std::shared_ptr<program> get_program() const;

    void set_textures(textures_t t);
    [[nodiscard]] const textures_t &get_textures() const;

  private:
    std::string m_material_library{"mtllib_undefined"};
    std::string m_name{"name_undefined"};
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texture_coords;
    std::vector<glm::vec3> m_vertex_normals;
    std::string m_used_material{"usemtl_undefined"};
    bool m_smooth_shading{false};
    std::vector<std::vector<face_t>> m_faces;

    vertex_array m_vertex_array;
    std::shared_ptr<program> m_program;
    textures_t m_textures;
};

} // namespace opengl_wrapper
