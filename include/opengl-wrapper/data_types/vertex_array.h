#pragma once

#include "buffer.h"
#include "vertex.h"
#include <ostream>
#include <vector>

namespace opengl_wrapper {

class vertex_array {
  public:
    /**
     * @brief Builds a large amount of vertex arrays at the same time.
     * @param amount Amount of vertex arrays.
     * @return Vector with the new vertex arrays.
     */
    static std::vector<vertex_array> build(size_t amount);

    /**
     * @brief Construct a new vertex_arrays object. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     *
     * @param size Number of vertex arrays to be generated.
     */
    explicit vertex_array(identifier_t id = 0);

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
    void bind() const;

    /**
     * @brief Gets the ids associated with this vertex_array.
     * @return Vertex array ids.
     */
    [[nodiscard]] identifier_t get_id() const;

    /**
     * @brief Creates and initializes a buffer object data storage. See
     * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     *
     * @param data Data to be stored.
     * @param usage Expected usage pattern of the data store.
     */
    void load(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices);

  private:
    identifier_t m_id;
    std::vector<buffer> m_buffers;
};

std::ostream &operator<<(std::ostream &os, const opengl_wrapper::vertex_array &va);

} // namespace opengl_wrapper
