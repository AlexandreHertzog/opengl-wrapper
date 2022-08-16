#ifndef OPENGL_WRAPPER_RENDERER_H
#define OPENGL_WRAPPER_RENDERER_H

#include "opengl-wrapper/wraps/buffer.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/vertex_arrays.h"
#include <map>
#include <memory>

namespace opengl_wrapper {

class shape;

class renderer {
  public:
    /**
     * @brief Adds first_vertices to the renderer ca cache.
     *
     * @param vertices Vertices array.
     * @param indices Indices for element drawing.
     * @param program Previously linked program index to be applied to the first_vertices.
     */
    void add_shape(shape s);

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
    std::vector<shape> shapes_;

    std::unique_ptr<vertex_arrays> vertex_arrays_;
    std::unique_ptr<buffer> vertex_buffer_;
};

} // namespace opengl_wrapper

#endif
