#include "face.h"
#include "utils/utils.h"

namespace opengl_wrapper {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"

std::istream &operator>>(std::istream &is, face_t &f) {
    std::string data;
    is >> data;

    const auto first_slash = data.find('/');
    if (std::string::npos == first_slash) {
        f.m_vertex_index = to_int(data.c_str(), data.size());
    } else {
        f.m_vertex_index = to_int(data.c_str(), first_slash);

        const auto second_slash = data.rfind('/');
        if (first_slash == second_slash) {
            f.m_texture_coord_index = to_int(data.c_str() + first_slash + 1, data.size() - first_slash);
        } else {
            if (second_slash != first_slash + 1) {
                f.m_texture_coord_index = to_int(data.c_str() + first_slash + 1, second_slash - (first_slash + 1));
            }
            f.m_normal_index = to_int(data.c_str() + second_slash + 1, data.size() - (second_slash + 1));
        }
    }

    return is;
}

#pragma clang diagnostic pop

bool operator==(const opengl_wrapper::face_t &lhs, const opengl_wrapper::face_t &rhs) {
    return (lhs.m_vertex_index == rhs.m_vertex_index) && (lhs.m_texture_coord_index == rhs.m_texture_coord_index) &&
           (lhs.m_normal_index == rhs.m_normal_index);
}

} // namespace opengl_wrapper
