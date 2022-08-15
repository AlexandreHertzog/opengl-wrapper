#ifndef OPENGL_WRAPPER_RENDERER_H
#define OPENGL_WRAPPER_RENDERER_H

#include "opengl-wrapper/wraps/buffer.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/vertex_arrays.h"
#include <map>
#include <memory>

namespace opengl_wrapper {

class renderer {
  public:
    /**
     * @brief Constructs a renderer.
     */
    renderer();

    /**
     * @brief Adds first_vertices to the renderer ca cache.
     *
     * @param vertices Vertices array.
     * @param indices Indices for element drawing.
     * @param program Previously linked program index to be applied to the first_vertices.
     */
    void add_vertices(std::vector<float> vertices, std::vector<unsigned int> indices, std::shared_ptr<program> program);

    /**
     * @brief Loads first_vertices into OpenGL. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml,
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
     */
    void load_vertices();

    /**
     * @brief Draws the given vertices with the given shaders with the associated program. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
     *
     * @param mode Mode bypass to the OpenGL draw function.
     */
    void draw();

  private:
    std::vector<std::vector<float>> vertices_;
    std::vector<std::vector<unsigned int>> indices_;

    std::map<int, std::shared_ptr<program>> vertices_program_map_;

    std::unique_ptr<vertex_arrays> vertex_arrays_;
    std::unique_ptr<buffer> vertex_buffer_;
    GLsizei vertex_count_;
    GLsizei indices_count_;
};

} // namespace opengl_wrapper

#endif
