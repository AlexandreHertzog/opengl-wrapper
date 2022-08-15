#ifndef OPENGL_WRAPPER_WRAPS_PROGRAM_H
#define OPENGL_WRAPPER_WRAPS_PROGRAM_H

#include <functional>
#include <glad/glad.h>
#include <vector>

#include "shader.h"

namespace opengl_wrapper {

class program {
  public:
    using usa_callback = std::function<void(program &program)>;

    /**
     * @brief Construct a new program object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateProgram.xhtml
     *
     */
    program();

    /**
     * @brief program move-constructor.
     *
     * @param other program to be emptied.
     */
    program(program &&other) noexcept;

    /**
     * @brief Destroy the program object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
     *
     */
    ~program();

    program(const program &) = delete;
    program &operator=(const program &) = delete;

    /**
     * @brief program move-assignment operator.
     *
     * @param other program to be emptied.
     * @return program& Reference to this.
     */
    program &operator=(program &&other) noexcept;

    /**
     * @brief Equality comparison operator.
     * @param other program to be compared with.
     * @return `true` if the programs have the same id, `false` otherwise.
     */
    bool operator==(const program &other) const;

    /**
     * @brief Attaches a shader to the program. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
     *
     * @param shader Compiled shader to be attached.
     */
    void add_shader(shader shader);

    /**
     * @brief Links the program with the previously defined shaders. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
     *
     * @throws GlError When the link fails.
     */
    void link();

    /**
     * @brief Sets the callback to be used after each time the program is activated.
     * @param callback Callback function.
     */
    void set_use_callback(usa_callback callback);

    /**
     * @brief Gets the reference for a Uniform variable in OpenGL. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
     * @param var_name Variable name.
     * @return The variable reference.
     */
    int get_uniform_location(const char *var_name) const;

    /**
     * @brief Uses this program. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
     *
     */
    void use();

  private:
    std::vector<shader> shaders_;
    unsigned int shader_count_;
    GLuint id_;
    bool linked_;
    usa_callback use_callback_;
};
} // namespace opengl_wrapper

#endif
