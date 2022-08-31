#pragma once

#include <array>
#include <iomanip>
#include <ostream>

namespace opengl_wrapper {

using identifier_t = unsigned;

using direction_t = std::array<float, 3>;
using position_t = std::array<float, 3>;
using tex_coord_t = std::array<float, 2>;

enum class buffer_target_t {
    undefined = -1,
    simple_array = 0x8892,
    element_array = 0x8893
};

inline std::ostream &operator<<(std::ostream &os, buffer_target_t t) {
    return os << std::hex << "0x" << static_cast<int>(t);
}

} // namespace opengl_wrapper
