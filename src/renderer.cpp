#include "renderer.h"

namespace opengl_wrapper {
Renderer::Renderer() : vao_(1), vbo_(1) {}

void Renderer::addVertices(GLsizeiptr vertices_size, const float *vertices) {
    vao_.bind(0);
    vbo_.bind(0, GL_ARRAY_BUFFER);

    vbo_.buffer(vertices_size, vertices, GL_STATIC_DRAW);

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

void Renderer::draw( // NOLINT(readability-convert-member-functions-to-static)
    GLenum mode) {
    glDrawArrays(mode, 0, 3);
}

} // namespace opengl_wrapper