#ifndef OPENGL_WRAPPER_WRAPS_SHADER_H
#define OPENGL_WRAPPER_WRAPS_SHADER_H

#include <glad/glad.h>
#include <string>

namespace opengl_wrapper {

class Shader {
  public:
    /**
     * @brief Construct a new Shader object then compiles it. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     *
     * @param type Type of Shader to be created.
     * @param source Source-code for the shader.
     * @throws GlError When the shader compilation fails.
     */
    Shader(GLenum type, const char *source);

    /**
     * @brief Shader move-constructor.
     *
     * @param other Shader to be emptied.
     */
    Shader(Shader &&other) noexcept;

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    /**
     * @brief Destroy the Shader object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
     *
     */
    ~Shader();

    /**
     * @brief Shader move-assignment operator.
     *
     * @param other Shader to be emptied.
     * @return Shader& Reference to this.
     */
    Shader &operator=(Shader &&other) noexcept;

    /**
     * @brief Gets the internal OpenGL shader ID.
     *
     * @return GLuint Shader ID.
     */
    [[nodiscard]] GLuint getId() const;

  private:
    GLuint id_{};
};

} // namespace opengl_wrapper

#endif
