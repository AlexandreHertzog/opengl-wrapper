#ifndef OPENGL_WRAPPER_SHAPE_H
#define OPENGL_WRAPPER_SHAPE_H

#include "opengl-wrapper/wraps/vertex_array.h"
#include "vertex.h"
#include <memory>
#include <vector>

namespace opengl_wrapper {

class program;
class texture;

class shape {
  public:
    unsigned add_vertex(vertex v);
    [[nodiscard]] const std::vector<vertex> &get_vertices() const;

    void set_draw_order(std::vector<unsigned> indices);
    [[nodiscard]] const std::vector<unsigned> &get_draw_order() const;

    void set_vertex_array(std::shared_ptr<vertex_array> va);
    [[nodiscard]] std::shared_ptr<vertex_array> get_vertex_array() const;

    void set_program(std::shared_ptr<program> p);
    [[nodiscard]] std::shared_ptr<program> get_program() const;

    void set_texture(std::shared_ptr<texture> t);
    [[nodiscard]] std::shared_ptr<texture> get_texture() const;

  private:
    std::vector<vertex> vertices_;
    std::vector<unsigned> indices_;
    std::shared_ptr<vertex_array> m_vertex_array;
    std::shared_ptr<program> program_;
    std::shared_ptr<texture> m_texture;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_SHAPE_H
