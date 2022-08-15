#ifndef OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H
#define OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H

#include <glad/glad.h>
#include <vector>

namespace opengl_wrapper {

class vertex_arrays {
  public:
    /**
     * @brief Construct a new vertex_arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     *
     * @param size Number of vertex arrays to be generated.
     */
    explicit vertex_arrays(int size);

    /**
     * @brief vertex_arrays move-constructor.
     *
     * @param other VectorArrays to be emptied.
     */
    vertex_arrays(vertex_arrays &&other) noexcept;

    vertex_arrays(const vertex_arrays &) = delete;
    vertex_arrays &operator=(const vertex_arrays &) = delete;

    /**
     * @brief Destroy the Vertex Arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
     *
     */
    ~vertex_arrays();

    /**
     * @brief vertex_arrays move-assignment operator.
     *
     * @param other vertex_arrays to be emptied.
     * @return vertex_arrays& Reference to this.
     */
    vertex_arrays &operator=(vertex_arrays &&other) noexcept;

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
