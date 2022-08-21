#pragma once

#include "opengl-wrapper/graphics/graphics.h"
#include <filesystem>
#include <ostream>
#include <string>

namespace opengl_wrapper {

class shader {
  public:
    /**
     * @brief Construct a new shader object then compiles it. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     *
     * @param type Type of shader to be created.
     * @param source Source-code for the shader.
     * @throws GlError When the shader compilation fails.
     */
    explicit shader(GLenum type, const char *source = nullptr);

    /**
     * @brief Construct a new shader object, reads its source from the filesystem then compiles it. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     *
     * @param type
     * @param shader_path
     */
    shader(GLenum type, const std::filesystem::path &shader_path);

    /**
     * @brief shader move-constructor.
     *
     * @param other shader to be emptied.
     */
    shader(shader &&other) noexcept;

    shader(const shader &) = delete;
    shader &operator=(const shader &) = delete;

    /**
     * @brief Destroy the shader object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
     *
     */
    ~shader();

    /**
     * @brief shader move-assignment operator.
     *
     * @param other shader to be emptied.
     * @return shader& Reference to this.
     */
    shader &operator=(shader &&other) noexcept;

    /**
     * @brief Gets the internal OpenGL shader ID.
     *
     * @return GLuint shader ID.
     */
    [[nodiscard]] GLuint get_id() const;

  private:
    GLuint m_id;

    void compile(const char *source);
};

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::shader &s);

} // namespace opengl_wrapper
