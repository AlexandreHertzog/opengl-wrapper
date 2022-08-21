#ifndef OPENGL_WRAPPER_TEXTURE_H
#define OPENGL_WRAPPER_TEXTURE_H

#include "opengl-wrapper/graphics/graphics.h"
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
    explicit texture(GLenum target = 0, GLuint id = 0, int unit = 0);

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

  private:
    GLuint m_id{};
    GLenum m_target{};
    int m_unit{};
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_TEXTURE_H
