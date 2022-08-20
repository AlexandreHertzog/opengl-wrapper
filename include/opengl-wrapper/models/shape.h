#ifndef OPENGL_WRAPPER_SHAPE_H
#define OPENGL_WRAPPER_SHAPE_H

#include "vertex.h"
#include <memory>
#include <vector>

namespace opengl_wrapper {

class program;

class shape {
  public:
    unsigned add_vertex(vertex v);
    [[nodiscard]] const std::vector<vertex> &get_vertices() const;

    void set_draw_order(std::vector<unsigned> indices);
    [[nodiscard]] const std::vector<unsigned> &get_draw_order() const;

    void set_vertex_array(int va);
    [[nodiscard]] int get_vertex_array() const;

    void set_program(std::shared_ptr<program> p);
    [[nodiscard]] std::shared_ptr<program> get_program() const;

  private:
    std::vector<vertex> vertices_;
    std::vector<unsigned> indices_;
    int vertex_array_{};
    std::shared_ptr<program> program_;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_SHAPE_H