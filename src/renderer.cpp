#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"
#include "renderer.h"

namespace opengl_wrapper {
Renderer::Renderer() : vertex_arrays_(1), vertex_buffer_(2) {}

void Renderer::addVertices(const std::vector<float> &vertices,
                           const std::vector<unsigned int> &indices) {

    vertex_arrays_.bind(0);

    vertex_buffer_.bind(0, GL_ARRAY_BUFFER);
    vertex_buffer_.buffer(
        static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
        vertices.data(), GL_STATIC_DRAW);

    if (!indices.empty()) {
        vertex_buffer_.bind(1, GL_ELEMENT_ARRAY_BUFFER);

        vertex_buffer_.buffer(
            static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
            indices.data(), GL_STATIC_DRAW);
    }

    vertex_count_ += static_cast<GLsizei>(vertices.size());
    indices_count_ += static_cast<GLsizei>(indices.size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

void Renderer::addShader(Shader shader) {
    program_.addShader(std::move(shader));
}

void Renderer::useProgram() {
    program_.link();
    program_.use();
}

void Renderer::draw(GLenum mode) {
    glDrawElements(mode, indices_count_, GL_UNSIGNED_INT, nullptr);
}

} // namespace opengl_wrapper
#pragma clang diagnostic pop