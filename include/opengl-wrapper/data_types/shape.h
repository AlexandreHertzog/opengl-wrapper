#pragma once

#include "face_vertex_indices.h"
#include "material.h"
#include "mesh.h"
#include "program.h"
#include "texture.h"
#include "transform.h"
#include "vertex.h"
#include "vertex_array.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>

namespace opengl_wrapper {

class program;

class shape {
  public:
    explicit shape(vertex_array va = vertex_array());
    shape(const shape &other);
    shape(shape &&other) noexcept;
    ~shape() = default;

    shape &operator=(const shape &other);
    shape &operator=(shape &&other) noexcept;

    void set_program(std::shared_ptr<program> p);

    void load_vertices();
    void bind();

    mesh &get_mesh();
    void set_mesh(mesh m);

    transform &get_transform();
    void set_transform(transform t);

    material &get_material();
    void set_material(material m);

    [[nodiscard]] std::vector<unsigned> serialize_draw_order() const;

    template <class... T> void set_uniform(const char *name, T... value) {
        assert(m_program);
        m_program->use(*this);
        m_program->set_uniform(name, value...);
    }

  private:
    mesh m_mesh;
    transform m_transform;
    material m_material;

    vertex_array m_vertex_array;
    std::shared_ptr<program> m_program;
};

} // namespace opengl_wrapper
