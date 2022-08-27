#pragma once

#include <istream>

namespace opengl_wrapper {

struct face_t {
    int m_vertex_index{};
    int m_texture_coord_index{};
    int m_normal_index{};
};

std::istream &operator>>(std::istream &is, face_t &f);
bool operator==(const opengl_wrapper::face_t &lhs, const opengl_wrapper::face_t &rhs);

} // namespace opengl_wrapper
