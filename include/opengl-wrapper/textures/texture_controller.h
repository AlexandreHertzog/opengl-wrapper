#ifndef OPENGL_WRAPPER_TEXTURE_CONTROLLER_H
#define OPENGL_WRAPPER_TEXTURE_CONTROLLER_H

#include "texture.h"
#include <filesystem>
#include <vector>

namespace opengl_wrapper {

class texture_controller {
  public:
    /**
     * @brief Gets texture at index.
     * @param index Index of the texture.
     * @return Texture.
     */
    texture &operator[](int index);

    /**
     * @brief Loads a texture from the filesystem.
     * @param path Path to texture.
     * @return Index of the laoded texture.
     */
    int add_texture(const std::filesystem::path &path);

  private:
    std::vector<texture> m_textures;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_TEXTURE_CONTROLLER_H
