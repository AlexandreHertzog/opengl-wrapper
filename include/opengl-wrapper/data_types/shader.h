#pragma once

#include "types.h"
#include <filesystem>
#include <ostream>
#include <string>

namespace opengl_wrapper {

class shader_t {
  public:
    /**
     * @brief Construct a new shader object then compiles it. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     *
     * @param type Type of shader to be created.
     * @param source Source-code for the shader.
     * @throws GlError When the shader compilation fails.
     */
    explicit shader_t(shader_type_t type, const char *source = nullptr);

    /**
     * @brief Construct a new shader object, reads its source from the filesystem then compiles it. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     *
     * @param type
     * @param shader_path
     */
    shader_t(shader_type_t type, const std::filesystem::path &shader_path);

    /**
     * @brief shader move-constructor.
     *
     * @param other shader to be emptied.
     */
    shader_t(shader_t &&other) noexcept;

    shader_t(const shader_t &) = delete;
    shader_t &operator=(const shader_t &) = delete;

    /**
     * @brief Destroy the shader object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
     *
     */
    ~shader_t();

    /**
     * @brief shader move-assignment operator.
     *
     * @param other shader to be emptied.
     * @return shader& Reference to this.
     */
    shader_t &operator=(shader_t &&other) noexcept;

    /**
     * @brief Gets the internal OpenGL shader ID.
     *
     * @return identifier_t shader ID.
     */
    [[nodiscard]] identifier_t get_id() const;

  private:
    identifier_t m_id{};

    void compile(const char *source, bool free_on_error);
    void gl_delete();
};

std::ostream &operator<<(std::ostream &os, const shader_t &s);

} // namespace opengl_wrapper
