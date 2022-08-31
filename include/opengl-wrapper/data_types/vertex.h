#pragma once

#include "opengl-wrapper/data_types/types.h"

namespace opengl_wrapper {

struct vertex {
    position_t m_pos;
    tex_coord_t m_tex;
    direction_t m_nor;
};

} // namespace opengl_wrapper
