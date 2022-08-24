#pragma once

#include "opengl-wrapper/graphics/graphics.h"
#include <filesystem>
#include <vector>

namespace opengl_wrapper {

class texture {
  public:
    /**
     * @brief Creates a texture object with the given target and id. If ID is zero the texture is created. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenTextures.xhtml
     * @param target OpenGL texture target.
     * @param id Texture ID.
     */
    explicit texture(int unit = 0, GLenum target = 0, GLuint id = 0);

    /**
     * @brief Texture destructor. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteTextures.xhtml
     */
    ~texture();

    /**
     * @brief Texture move-constructor.
     * @param other Texture to be emptied.
     */
    texture(texture &&other) noexcept;

    /**
     * @brief Texture move-assignment operator.
     * @param other Texture to be emptied.
     * @return Reference to this.
     */
    texture &operator=(texture &&other) noexcept;

    texture(const texture &) = delete;
    texture &operator=(const texture &) = delete;

    /**
     * @brief Binds the texture in OpenGL. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindTexture.xhtml
     */
    void bind();

    /**
     * @brief Sets a texture parameter. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
     * @param pname Parameter name.
     * @param param Parameter value.
     */
    void set_parameter(GLenum pname, GLint param);

    /**
     * @brief Sets the texture image. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
     * @param level Specifies the level-of-detail number.
     * @param internalformat Specifies the number of color components in the texture_coord.
     * @param width Specifies the width of the texture_coord image.
     * @param height Specifies the height of the texture_coord image.
     * @param border This value must be 0.
     * @param format Specifies the format of the pixel data.
     * @param type Specifies the data type of the pixel data.
     * @param data Specifies a pointer to the image data in memory.
     */
    void set_image(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format,
                   GLenum type, const void *data);

    /**
     * @brief Generates the texture mipmap. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenerateMipmap.xhtml
     */
    void generate_mipmap();

    void set_image_from_path(const std::filesystem::path &path);

    [[nodiscard]] GLuint get_id() const;
    [[nodiscard]] GLenum get_target() const;
    void set_unit(int unit);
    [[nodiscard]] int get_unit() const;

  private:
    GLuint m_id{};
    GLenum m_target{};
    int m_unit{};
};

std::ostream &operator<<(std::ostream &os, const texture &t);

} // namespace opengl_wrapper
