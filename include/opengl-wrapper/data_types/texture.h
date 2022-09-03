#pragma once

#include "opengl-wrapper/graphics/graphics.h"
#include "types.h"
#include <cassert>
#include <filesystem>
#include <vector>

namespace opengl_wrapper {

class texture {
  public:
    using pointer_t = std::shared_ptr<texture>;

    static pointer_t build(const std::filesystem::path &path, int unit);

    /**
     * @brief Creates a texture object with the given target and id. If ID is zero the texture is created. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenTextures.xhtml
     * @param target OpenGL texture target.
     * @param id Texture ID.
     */
    explicit texture(int unit, texture_target_t target = texture_target_t::undefined, identifier_t id = 0);

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
     * @brief Sets the texture image. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
     * @param width Specifies the width of the texture_coord image.
     * @param height Specifies the height of the texture_coord image.
     * @param format Specifies the format of the pixel data.
     * @param data Specifies a pointer to the image data in memory.
     */
    void set_image(size_t width, size_t height, texture_format_t format, const unsigned char *data);

    /**
     * @brief Generates the texture mipmap. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenerateMipmap.xhtml
     */
    void generate_mipmap();

    void set_image_from_path(const std::filesystem::path &path);

    [[nodiscard]] identifier_t get_id() const;
    [[nodiscard]] texture_target_t get_target() const;
    [[nodiscard]] int get_unit() const;

  private:
    identifier_t m_id{};
    texture_target_t m_target{};
    int m_unit{-1};

    void gl_delete();

    /**
     * @brief Sets a texture parameter. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
     * @param param Parameter name.
     * @param value Parameter value.
     */
    template <class T> void set_parameter(texture_parameter_t param, T value) {
        assert(m_id != 0);
        assert(m_target != texture_target_t::undefined);
        graphics::instance().set_parameter(param, value);
    }
};

std::ostream &operator<<(std::ostream &os, const texture &t);

} // namespace opengl_wrapper
