#ifndef OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H
#define OPENGL_WRAPPER_WRAPS_VERTEX_ARRAYS_H

#include "buffer.h"
#include "opengl-wrapper/graphics/graphics.h"
#include "opengl-wrapper/models/vertex.h"
#include <array>
#include <ostream>
#include <vector>

namespace opengl_wrapper {

class vertex_array {
  public:
    /**
     * @brief Construct a new vertex_arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     *
     * @param size Number of vertex arrays to be generated.
     */
    explicit vertex_array(std::vector<std::shared_ptr<buffer>> buffers = {}, GLuint id = 0);

    /**
     * @brief vertex_arrays move-constructor.
     *
     * @param other VectorArrays to be emptied.
     */
    vertex_array(vertex_array &&other) noexcept;

    vertex_array(const vertex_array &) = delete;
    vertex_array &operator=(const vertex_array &) = delete;

    /**
     * @brief Destroy the Vertex Arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
     *
     */
    ~vertex_array();

    /**
     * @brief vertex_arrays move-assignment operator.
     *
     * @param other vertex_arrays to be emptied.
     * @return vertex_arrays& Reference to this.
     */
    vertex_array &operator=(vertex_array &&other) noexcept;

    /**
     * @brief Binds the VertexArray object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
     *
     * @param index Index of the VertexArray to be bound.
     */
    void bind();

    /**
     * @brief Gets the ids associated with this vertex_array.
     * @return Vertex array ids.
     */
    [[nodiscard]] GLuint get_id() const;

    /**
     * @brief Creates and initializes a buffer object data storage. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     *
     * @param data Data to be stored.
     * @param usage Expected usage pattern of the data store.
     */
    void load(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices, GLenum usage);

  private:
    GLuint m_id;
    std::vector<std::shared_ptr<buffer>> m_buffers;
};

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::vertex_array &va);

} // namespace opengl_wrapper

#endif
