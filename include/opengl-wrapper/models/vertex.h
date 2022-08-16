//
// Created by alex on 16-08-2022.
//

#ifndef OPENGL_WRAPPER_VERTEX_H
#define OPENGL_WRAPPER_VERTEX_H

#include <array>

namespace opengl_wrapper {

struct vertex {
    using position = std::array<float, 3>;
    using color = std::array<float, 3>;

    position pos_;
    color col_;
};

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_VERTEX_H
