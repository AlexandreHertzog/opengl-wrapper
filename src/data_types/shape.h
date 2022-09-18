#pragma once

#include "data_types/face.h"
#include "data_types/mesh.h"
#include "data_types/types.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <opengl-cpp/program.h>
#include <opengl-cpp/texture.h>
#include <opengl-cpp/vertex_array.h>
#include <vector>

namespace game_engine {

class shape_t {
  public:
    explicit shape_t(opengl_cpp::vertex_array_t va);
    shape_t(shape_t &&other) noexcept;
    shape_t &operator=(shape_t &&other) noexcept;
    ~shape_t() = default;

    shape_t &operator=(const shape_t &other) = delete;
    shape_t(const shape_t &other) = delete;

    void load_vertices();
    void bind();
    [[nodiscard]] glm::mat4 model_transformations() const;

    mesh_t &get_mesh();
    void set_mesh(mesh_t m);

    transform_t &get_transform();
    void set_transform(transform_t t);

    material_t &get_material();
    void set_material(material_t m);

  private:
    mesh_t m_mesh;
    transform_t m_transform;
    material_t m_material;
    opengl_cpp::vertex_array_t m_vertex_array;
};

} // namespace game_engine
