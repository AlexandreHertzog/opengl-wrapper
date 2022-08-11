#include "renderer.h"
#include <boost/log/trivial.hpp>
#include <cassert>

namespace opengl_wrapper {

void Renderer::addVertices(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int program_index) {
    vertices_.emplace_back(std::move(vertices));
    indices_.emplace_back(std::move(indices));

    assert(program_index < linked_programs_.size());
    program_vertices_map_[program_index] = static_cast<int>(vertices_.size() - 1);
}

void Renderer::loadVertices() {
    assert(vertices_.size() == indices_.size());

    vertex_arrays_ = std::make_unique<VertexArrays>(vertices_.size());
    vertex_buffer_ = std::make_unique<Buffer>(vertices_.size() * 2);

    for (int i = 0; i < vertices_.size(); i++) {
        vertex_arrays_->bind(i);
        vertex_buffer_->bind(2 * i, GL_ARRAY_BUFFER);

        vertex_buffer_->buffer(static_cast<GLsizeiptr>(vertices_[i].size() * sizeof(float)), vertices_[i].data(),
                               GL_STATIC_DRAW);

        if (!indices_[i].empty()) {
            vertex_buffer_->bind(2 * i + 1, GL_ELEMENT_ARRAY_BUFFER);

            vertex_buffer_->buffer(static_cast<GLsizeiptr>(indices_[i].size() * sizeof(unsigned int)),
                                   indices_[i].data(), GL_STATIC_DRAW);
        }

        vertex_count_ += static_cast<GLsizei>(vertices_[i].size());
        indices_count_ += static_cast<GLsizei>(indices_[i].size());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }
}

void Renderer::addShader(Shader shader) {
    current_program_.addShader(std::move(shader));
}

unsigned int Renderer::linkProgram() {
    assert(current_program_.getShaderCount() > 0);

    current_program_.link();
    linked_programs_.emplace_back(std::move(current_program_));
    return linked_programs_.size() - 1;
}

void Renderer::draw() {
    if (current_program_.underConstruction()) {
        BOOST_LOG_TRIVIAL(warning) << "Entering drawing loop while a program is under construction";
    }

    for (const auto &program_index : program_vertices_map_) {
        linked_programs_[program_index.first].use();
        vertex_arrays_->bind(program_index.second);

        glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, nullptr);
    }
}

} // namespace opengl_wrapper
