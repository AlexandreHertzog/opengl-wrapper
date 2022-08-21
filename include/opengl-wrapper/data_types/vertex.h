#pragma once

#include <array>

namespace opengl_wrapper {

struct vertex {
    using position = std::array<float, 3>;
    using color = std::array<float, 3>;
    using texture_coord = std::array<float, 2>;

    position m_pos;
    color m_col;
    texture_coord m_tex;
};

} // namespace opengl_wrapper
