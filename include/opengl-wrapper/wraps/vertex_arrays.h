#ifndef OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H
#define OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H

#include <glad/glad.h>
#include <vector>

namespace opengl_wrapper {
class VertexArrays {
  public:
    /**
     * @brief Construct a new VertexArrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     *
     * @param size Number of vertex arrays to be generated.
     */
    explicit VertexArrays(int size);

    /**
     * @brief VertexArrays move-constructor.
     *
     * @param other VectorArrays to be emptied.
     */
    VertexArrays(VertexArrays &&other) noexcept;

    VertexArrays(const VertexArrays &) = delete;
    VertexArrays &operator=(const VertexArrays &) = delete;

    /**
     * @brief Destroy the Vertex Arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
     *
     */
    ~VertexArrays();

    /**
     * @brief VertexArrays move-assignment operator.
     *
     * @param other VertexArrays to be emptied.
     * @return VertexArrays& Reference to this.
     */
    VertexArrays &operator=(VertexArrays &&other) noexcept;

    /**
     * @brief Binds the VertexArray object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
     *
     * @param index Index of the VertexArray to be bound.
     */
    void bind(int index);

  private:
    std::vector<GLuint> ids_;
};
} // namespace opengl_wrapper

#endif
