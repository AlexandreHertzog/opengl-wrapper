#include "renderer.h"
#include <boost/log/trivial.hpp>
#include <cassert>

namespace opengl_wrapper {

Renderer::Renderer() : vertex_count_(0), indices_count_(0) {
}

void Renderer::addVertices(std::vector<float> vertices, std::vector<unsigned int> indices,
                           std::shared_ptr<Program> program) {

    vertices_.emplace_back(std::move(vertices));
    indices_.emplace_back(std::move(indices));

    assert(std::find(linked_programs_.begin(), linked_programs_.end(), program) != linked_programs_.end());
    vertices_program_map_[static_cast<int>(vertices_.size() - 1)] = std::move(program);
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr); // NOLINT
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),  // NOLINT
                              reinterpret_cast<void *>(3 * sizeof(float))); // NOLINT
        glEnableVertexAttribArray(1);
    }
}

void Renderer::addShader(Shader shader) {
    if (!current_program_) {
        current_program_ = std::make_shared<Program>();
    }
    current_program_->addShader(std::move(shader));
}

std::shared_ptr<Program> Renderer::linkProgram() {
    assert(current_program_);
    assert(current_program_->getShaderCount() > 0);

    current_program_->link();
    linked_programs_.emplace_back(std::move(current_program_));
    return linked_programs_.back();
}

void Renderer::draw() {
    if (current_program_ && current_program_->underConstruction()) {
        BOOST_LOG_TRIVIAL(warning) << "Entering drawing loop while a program is under construction";
    }

    for (const auto &program_index : vertices_program_map_) {
        program_index.second->use();
        vertex_arrays_->bind(program_index.first);

        glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, nullptr);
    }
}

} // namespace opengl_wrapper
