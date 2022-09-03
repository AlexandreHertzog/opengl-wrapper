#pragma once

#include "face_vertex_indices.h"
#include "types.h"
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
    mesh(const mesh &other) = default;
    ~mesh() = default;

    mesh &operator=(mesh &&other) noexcept;
    mesh &operator=(const mesh &other);

    [[nodiscard]] const std::vector<vertex> &get_vertices() const;
    [[nodiscard]] const std::string &get_name() const;

  private:
    std::string m_material_library{"mtllib_undefined"};
    std::string m_name{"name_undefined"};
    std::vector<position_t> m_vertices;
    std::vector<tex_coord_t> m_texture_coords;
    std::vector<direction_t> m_vertex_normals;
    std::string m_used_material{"usemtl_undefined"};
    bool m_smooth_shading{false};
    std::vector<std::vector<face_t>> m_faces;

    std::vector<vertex> m_cached_vertices;

    void cache_vertices();
};

} // namespace opengl_wrapper
