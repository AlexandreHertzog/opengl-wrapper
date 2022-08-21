#pragma once

#include "opengl-wrapper/data_types/program.h"
#include "texture_controller.h"
#include <filesystem>
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

    /**
     * @brief Loads a texture from the filesystem into the renderer.
     * @param path Path to texture.
     * @return Pointer of the new texture.
     */
    std::shared_ptr<texture> add_texture(const std::filesystem::path &path, int unit);

  private:
    std::vector<shape> m_shapes;
    texture_controller m_textures;
};

} // namespace opengl_wrapper
