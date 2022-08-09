#ifndef OPENGL_WRAPPER_RENDERER_H
#define OPENGL_WRAPPER_RENDERER_H

#include "opengl-wrapper/wraps/buffer.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/vertex_arrays.h"

namespace opengl_wrapper {

class Window;

class Renderer {
  public:
    /**
     * @brief Constructs a renderer.
     */
    Renderer();

    /**
     * @brief Adds vertices to the renderer. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml,
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
     *
     * @param vertices Vertices array.
     * @param indices Indices for element drawing.
     */
    void addVertices(const std::vector<float> &vertices,
                     const std::vector<unsigned int> &indices);

    /**
     * @brief Adds a shader to the renderer.
     * @param shader Shader to be added.
     */
    void addShader(Shader shader);

    /**
     * @brief Orders the renderer to compile and use the program.
     */
    void useProgram();

    /**
     * @brief Draws the given vertices with the given shaders. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
     *
     * @param mode Mode bypass to the OpenGL draw function.
     */
    void draw(GLenum mode);

  private:
    Program program_;
    VertexArrays vertex_arrays_;
    Buffer vertex_buffer_;
    GLsizei vertex_count_{};
    GLsizei indices_count_{};
};

} // namespace opengl_wrapper

#endif
