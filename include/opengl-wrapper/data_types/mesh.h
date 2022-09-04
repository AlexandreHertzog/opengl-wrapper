#pragma once

#include "face.h"
#include "types.h"
#include <filesystem>
#include <string>
#include <vector>

namespace opengl_wrapper {

class mesh_t {
  public:
    mesh_t() = default;
    explicit mesh_t(const std::filesystem::path &wavefront_object_path);
    mesh_t(mesh_t &&other) noexcept;
    mesh_t(const mesh_t &other) = default;
    ~mesh_t() = default;

    mesh_t &operator=(mesh_t &&other) noexcept;
    mesh_t &operator=(const mesh_t &other);

    [[nodiscard]] const std::vector<vertex_t> &get_vertices() const;
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

    std::vector<vertex_t> m_cached_vertices;

    void cache_vertices();
};

} // namespace opengl_wrapper
