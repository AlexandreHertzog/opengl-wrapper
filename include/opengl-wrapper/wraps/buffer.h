#ifndef OPENGL_WRAPPER_WRAPS_BUFFER_H
#define OPENGL_WRAPPER_WRAPS_BUFFER_H

#include <glad/glad.h>
#include <vector>

namespace opengl_wrapper {

class buffer {
  public:
    /**
     * @brief Construct a new buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
     *
     * @param size The number of buffer names to be generated.
     */
    explicit buffer(int size);

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
     *
     * @param index Index of the buffer to be bound.
     * @param target Target to which the load object is bound.
     */
    void bind(int index, GLenum target);

    /**
     * @brief Creates and initializes a buffer object data storage. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     *
     * @param size Size of bytes of the buffer to be stored.
     * @param data Pointer to the data to be stored.
     * @param usage Expected usage pattern of the data store.
     */
    void load(GLsizeiptr size, const void *data, GLenum usage);

  private:
    std::vector<GLuint> ids_;
    GLenum target_;
};

} // namespace opengl_wrapper

#endif
