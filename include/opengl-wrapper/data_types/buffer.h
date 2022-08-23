#pragma once

#include "opengl-wrapper/graphics/graphics.h"
#include <ostream>
#include <vector>

namespace opengl_wrapper {

class buffer {
  public:
    /**
     * @brief Builds a large amount of buffers at the same time.
     * @param amount Amount of buffers.
     * @return Vector with the new buffers.
     */
    static std::vector<buffer> build(size_t amount);

    /**
     * @brief Construct a new buffer object.
     *
     * @param id The buffer id.
     * @param target The buffer target.
     */
    explicit buffer(GLuint id = 0, GLenum target = 0);

    /**
     * @brief buffer move-constructor.
     *
     * @param other buffer to be emptied.
     */
    buffer(buffer &&other) noexcept;

    buffer(const buffer &) = delete;
    buffer &operator=(const buffer &) = delete;

    /**
     * @brief Destroy the buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
     *
     */
    ~buffer();

    /**
     * @brief buffer move-assignment operator.
     *
     * @param other buffer to be emptied.
     * @return buffer& Reference to this.
     */
    buffer &operator=(buffer &&other) noexcept;

    /**
     * @brief Binds the indicated buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
     */
    void bind();

    /**
     * @brief Creates and initializes a buffer object data storage. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     *
     * @param data Data to be stored.
     * @param usage Expected usage pattern of the data store.
     */
    template <class TYPE> void load(const std::vector<TYPE> &data, GLenum usage) {
        graphics::instance().gl_buffer_data(m_target, data.size() * sizeof(TYPE), data.data(), usage);
    }

    /**
     * @brief Get the buffer ids associated with this object.
     * @return Buffer ids.
     */
    [[nodiscard]] GLuint get_id() const;

    /**
     * @brief Gets the buffer target associated with this object.
     * @return Buffer target.
     */
    [[nodiscard]] GLenum get_target() const;

    /**
     * @brief Sets the buffer target.
     * @param target Buffer target.
     */
    void set_target(GLenum target);

  private:
    GLuint m_id;
    GLenum m_target;
};

std::ostream &operator<<(std::ostream &s, const opengl_wrapper::buffer &b);

} // namespace opengl_wrapper
