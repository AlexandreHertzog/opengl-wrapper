#pragma once

#include <istream>

namespace game_engine {

struct face_t {
    int m_vertex_index{};
    int m_texture_coord_index{};
    int m_normal_index{};
};

std::istream &operator>>(std::istream &is, face_t &f);
bool operator==(const game_engine::face_t &lhs, const game_engine::face_t &rhs);

} // namespace game_engine
