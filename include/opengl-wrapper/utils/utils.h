#pragma once

#include "exception.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <ostream>
#include <sstream>
#include <vector>

namespace opengl_wrapper {

template <typename TYPE> std::string between(const TYPE &t, char begin, char end) {
    std::stringstream out_stream;
    out_stream << begin << t << end;
    return out_stream.str();
}

template <typename TYPE> std::string braces(const TYPE &t) {
    return between(t, '{', '}');
}

template <typename TYPE> std::string parenthesis(const TYPE &t) {
    return between(t, '(', ')');
}

template <typename TYPE> std::ostream &operator<<(std::ostream &stream, const std::vector<TYPE> &vector) {
    using opengl_wrapper::operator<<;

    std::stringstream res;

    bool first = true;

    std::for_each(vector.begin(), vector.end(), [&](const TYPE &value) {
        if (!first) {
            res << ", ";
        }
        first = false;
        res << value;
    });

    return stream << braces(res.str());
}

inline std::istream &operator>>(std::istream &is, glm::vec2 &vec) {
    return is >> vec.x >> vec.y;
}

inline std::istream &operator>>(std::istream &is, glm::vec3 &vec) {
    return is >> vec.x >> vec.y >> vec.z;
}

template <class TYPE> std::istream &operator>>(std::istream &is, std::vector<TYPE> &out) {
    while (!is.eof()) {
        TYPE data;
        is >> data;
        out.emplace_back(std::move(data));
    }
    return is;
}

inline int to_int(const char *s, size_t len, int base = 10) {
    assert(nullptr != s);
    assert(len > 0);

    char *end{};
    const int ret = std::strtol(s, &end, base);
    if (end == s) {
        throw exception("Invalid to_int parameter: " + std::string(s));
    }
    return ret;
}

} // namespace opengl_wrapper
