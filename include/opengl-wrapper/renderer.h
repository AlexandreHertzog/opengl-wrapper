#ifndef OPENGL_WRAPPER_RENDERER_H
#define OPENGL_WRAPPER_RENDERER_H

#include "opengl-wrapper/wraps/buffer.h"
#include "opengl-wrapper/wraps/program.h"
#include "opengl-wrapper/wraps/vertex_arrays.h"
#include <map>
#include <memory>

namespace opengl_wrapper {

class WindowManager;

class Renderer {
  public:
    /**
     * @brief Constructs a renderer.
     */
    Renderer() = default;

    /**
     * @brief Adds a shader to the renderer.
     * @param shader Shader to be added.
     */
    void addShader(Shader shader);

    /**
     * @brief Links the program with the previously passed shaders.
     * @return The program index in the renderer.
     */
    unsigned int linkProgram();

    /**
     * @brief Adds first_vertices to the renderer ca cache.
     *
     * @param vertices Vertices array.
     * @param indices Indices for element drawing.
     * @param program_index Previously linked program index to be applied to the first_vertices.
     */
    void addVertices(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int program_index);

    /**
     * @brief Loads first_vertices into OpenGL. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml,
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
     */
    void loadVertices();

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

    Program current_program_;
    std::vector<Program> linked_programs_;
    std::map<unsigned int, int> program_vertices_map_;

    std::unique_ptr<VertexArrays> vertex_arrays_;
    std::unique_ptr<Buffer> vertex_buffer_;
    GLsizei vertex_count_{};
    GLsizei indices_count_{};
};

} // namespace opengl_wrapper

#endif
