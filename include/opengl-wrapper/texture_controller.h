#ifndef OPENGL_WRAPPER_TEXTURE_CONTROLLER_H
#define OPENGL_WRAPPER_TEXTURE_CONTROLLER_H

#include "opengl-wrapper/data_types/texture.h"
#include <filesystem>
#include <memory>
#include <vector>

namespace opengl_wrapper {

class texture_controller {
  public:
    /**
     * @brief Gets texture at index.
     * @param index Index of the texture.
     * @return Texture pointer.
     */
    std::shared_ptr<texture> &operator[](int index);

    /**
     * @brief Loads a texture from the filesystem.
     * @param path Path to texture.
     * @return Pointer of the laoded texture.
     */
    std::shared_ptr<texture> add_texture(const std::filesystem::path &path, int unit);

  private:
    std::vector<std::shared_ptr<texture>> m_textures;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_TEXTURE_CONTROLLER_H
