#pragma once

#include "vertex.h"
#include "vertex_array.h"
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

    shape() = default;
    shape(const shape &other);
    shape(shape &&other) noexcept;
    ~shape() = default;

    shape &operator=(const shape &other);
    shape &operator=(shape &&other) noexcept;

    unsigned add_vertex(vertex v);
    [[nodiscard]] const std::vector<vertex> &get_vertices() const;

    void set_draw_order(std::vector<unsigned> indices);
    [[nodiscard]] const std::vector<unsigned> &get_draw_order() const;

    void set_vertex_array(vertex_array va);
    [[nodiscard]] vertex_array &get_vertex_array();

    void set_program(std::shared_ptr<program> p);
    [[nodiscard]] std::shared_ptr<program> get_program() const;

    void set_textures(textures_t t);
    [[nodiscard]] const textures_t &get_textures() const;

  private:
    std::vector<vertex> m_vertices;
    std::vector<unsigned> m_indices;
    vertex_array m_vertex_array;
    std::shared_ptr<program> m_program;
    textures_t m_textures;
};

} // namespace opengl_wrapper
