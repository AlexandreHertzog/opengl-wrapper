#ifndef OPENGL_WRAPPER_WRAPS_BUFFER_H
#define OPENGL_WRAPPER_WRAPS_BUFFER_H

#include <glad/glad.h>
#include <vector>

namespace opengl_wrapper {
class Buffer {
  public:
    /**
     * @brief Construct a new Buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
     *
     * @param size The number of buffer names to be generated.
     */
    explicit Buffer(int size);

    /**
     * @brief Buffer move-constructor.
     *
     * @param other Buffer to be emptied.
     */
    Buffer(Buffer &&other) noexcept;

    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    /**
     * @brief Destroy the Buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
     *
     */
    ~Buffer();

    /**
     * @brief Buffer move-assignment operator.
     *
     * @param other Buffer to be emptied.
     * @return Buffer& Reference to this.
     */
    Buffer &operator=(Buffer &&other) noexcept;

    /**
     * @brief Binds the indicated buffer object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
     *
     * @param index Index of the Buffer to be bound.
     * @param target Target to which the buffer object is bound.
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
    void buffer(GLsizeiptr size, const void *data, GLenum usage);

  private:
    std::vector<GLuint> ids_;
    GLenum target_;
};
} // namespace opengl_wrapper

#endif
