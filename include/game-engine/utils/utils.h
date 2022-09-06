#pragma once

#include "exception.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <ostream>
#include <sstream>
#include <vector>

namespace game_engine {

template <typename type_t> std::string between(const type_t &t, char begin, char end) {
    std::stringstream out_stream;
    out_stream << begin << t << end;
    return out_stream.str();
}

template <typename type_t> std::string braces(const type_t &t) {
    return between(t, '{', '}');
}

template <typename type_t> std::string parenthesis(const type_t &t) {
    return between(t, '(', ')');
}

template <typename type_t> std::ostream &operator<<(std::ostream &stream, const std::vector<type_t> &vector) {
    using game_engine::operator<<;

    std::stringstream res;

    bool first = true;

    std::for_each(vector.begin(), vector.end(), [&](const type_t &value) {
        if (!first) {
            res << ", ";
        }
        first = false;
        res << value;
    });

    return stream << braces(res.str());
}

inline std::istream &operator>>(std::istream &is, glm::vec2 &vec) {
    return is >> vec.x >> vec.y; // NOLINT(cppcoreguidelines-pro-type-union-access)
}

inline std::istream &operator>>(std::istream &is, glm::vec3 &vec) {
    return is >> vec.x >> vec.y >> vec.z; // NOLINT(cppcoreguidelines-pro-type-union-access)
}

template <class type_t> std::istream &operator>>(std::istream &is, std::vector<type_t> &out) {
    while (!is.eof()) {
        type_t data;
        is >> data;
        out.emplace_back(std::move(data));
    }
    return is;
}

template <class type_t, size_t size> std::istream &operator>>(std::istream &is, std::array<type_t, size> &out) {
    for (auto &o : out) {
        is >> o;
    }
    return is;
}

inline int to_int(const char *s, size_t len, int base = 10) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    assert(nullptr != s);
    assert(len > 0);

    char *end{};
    const auto ret = static_cast<int>(std::strtol(s, &end, base));
    if (end == s) {
        throw exception_t("Invalid to_int parameter: " + std::string(s));
    }
    return ret;
}

} // namespace game_engine
