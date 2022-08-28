#pragma once

#include "face_vertex_indices.h"
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
    using textures_t = std::vector<texture::pointer_t>;

    explicit shape(vertex_array va = vertex_array());
    shape(const shape &other);
    shape(shape &&other) noexcept;
    ~shape() = default;

    shape &operator=(const shape &other);
    shape &operator=(shape &&other) noexcept;

    void set_program(std::shared_ptr<program> p);
    void set_textures(textures_t t);
    void add_texture(texture::pointer_t t);
    [[nodiscard]] const textures_t &get_textures() const;

    void load_vertices();
    void bind();

    mesh &get_mesh();
    void set_mesh(mesh m);

    transform &get_transform();
    void set_transform(transform t);

    [[nodiscard]] std::vector<unsigned> serialize_draw_order() const;

    void set_specular(float v);
    [[nodiscard]] float get_specular() const;

    void set_shininess(float v);
    [[nodiscard]] float get_shininess() const;

    template <class... T> void set_uniform(const char *name, T... value) {
        assert(m_program);
        m_program->use(*this);
        m_program->set_uniform(name, value...);
    }

  private:
    mesh m_mesh;
    transform m_transform;
    vertex_array m_vertex_array;
    std::shared_ptr<program> m_program;
    textures_t m_textures;

    float m_specular{0.5F};
    float m_shininess{32.0F};
};

} // namespace opengl_wrapper
