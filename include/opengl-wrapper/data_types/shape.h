#pragma once

#include "face.h"
#include "mesh.h"
#include "program.h"
#include "texture.h"
#include "vertex_array.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>

namespace opengl_wrapper {

class program_t;

class shape_t {
  public:
    explicit shape_t(vertex_array_t va = vertex_array_t());
    shape_t(const shape_t &other);
    shape_t(shape_t &&other) noexcept;
    ~shape_t() = default;

    shape_t &operator=(const shape_t &other);
    shape_t &operator=(shape_t &&other) noexcept;

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
    vertex_array_t m_vertex_array;
};

} // namespace opengl_wrapper
