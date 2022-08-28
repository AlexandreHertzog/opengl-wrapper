#pragma once

#include "face_vertex_indices.h"
#include "glm/glm.hpp"
#include "vertex.h"
#include <filesystem>
#include <string>
#include <vector>

namespace opengl_wrapper {

class mesh {
  public:
    mesh() = default;
    explicit mesh(const std::filesystem::path &wavefront_object_path);
    mesh(mesh &&other) noexcept;
    mesh(const mesh &other);
    ~mesh() = default;

    mesh &operator=(mesh &&other) noexcept;
    mesh &operator=(const mesh &other);

    [[nodiscard]] const std::vector<vertex> &get_vertices() const;

  private:
    std::string m_material_library{"mtllib_undefined"};
    std::string m_name{"name_undefined"};
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texture_coords;
    std::vector<glm::vec3> m_vertex_normals;
    std::string m_used_material{"usemtl_undefined"};
    bool m_smooth_shading{false};
    std::vector<std::vector<face_t>> m_faces;

    std::vector<vertex> m_cached_vertices;

    void cache_vertices();
};

} // namespace opengl_wrapper
