#ifndef OPENGL_WRAPPER_WRAPS_PROGRAM_H
#define OPENGL_WRAPPER_WRAPS_PROGRAM_H

#include <glad/glad.h>
#include <vector>

#include "shader.h"

namespace opengl_wrapper {
class Program {
  public:
    /**
     * @brief Construct a new Program object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateProgram.xhtml
     *
     */
    Program();

    /**
     * @brief Program move-constructor.
     *
     * @param other Program to be emptied.
     */
    Program(Program &&other) noexcept;

    /**
     * @brief Destroy the Program object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
     *
     */
    ~Program();

    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;

    /**
     * @brief Program move-assignment operator.
     *
     * @param other Program to be emptied.
     * @return Program& Reference to this.
     */
    Program &operator=(Program &&other) noexcept;

    /**
     * @brief Attaches a shader to the Program. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
     *
     * @param shader Compiled Shader to be attached.
     */
    void addShader(Shader shader);

    /**
     * @brief Links the program with the previously defined shaders. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
     *
     * @throws GlError When the link fails.
     */
    void link();

    /**
     * @brief Uses this Program. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
     *
     */
    void use();

  private:
    std::vector<Shader> shaders_;
    GLuint id_{};
};
} // namespace opengl_wrapper

#endif
