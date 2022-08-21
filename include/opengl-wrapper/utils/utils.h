#ifndef OPENGL_WRAPPER_UTILS_UTILS_H
#define OPENGL_WRAPPER_UTILS_UTILS_H

#include <algorithm>
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

} // namespace opengl_wrapper

#endif // OPENGL_WRAPPER_UTILS_UTILS_H
