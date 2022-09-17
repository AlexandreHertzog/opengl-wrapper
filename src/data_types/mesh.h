#pragma once

#include "data_types/face.h"
#include <filesystem>
#include <opengl-cpp/vertex_array.h>
#include <string>
#include <vector>

namespace game_engine {

class mesh_t {
  public:
    mesh_t() = default;
    explicit mesh_t(const std::filesystem::path &wavefront_object_path);
    mesh_t(mesh_t &&other) noexcept;
    mesh_t(const mesh_t &other) = default;
    ~mesh_t() = default;

    mesh_t &operator=(mesh_t &&other) noexcept;
    mesh_t &operator=(const mesh_t &other);

    [[nodiscard]] const std::vector<opengl_cpp::vertex_t> &get_vertices() const;
    [[nodiscard]] const std::string &get_name() const;

  private:
    std::string m_material_library{"mtllib_undefined"};
    std::string m_name{"name_undefined"};
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texture_coords;
    std::vector<glm::vec3> m_vertex_normals;
    std::string m_used_material{"usemtl_undefined"};
    bool m_smooth_shading{false};
    std::vector<std::vector<face_t>> m_faces;

    std::vector<opengl_cpp::vertex_t> m_cached_vertices;

    void cache_vertices();
};

} // namespace game_engine
