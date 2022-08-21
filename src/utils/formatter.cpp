#include "formatter.h"

#include <iomanip>
#include <sstream>

namespace opengl_wrapper {
std::string formatter::format_glfw_error_code(int error_code) {
    constexpr auto glfw_error_str_length = 8;

    std::stringstream stream;
    stream << "0x" << std::internal << std::hex << std::setw(glfw_error_str_length) << std::setfill('0') << error_code;
    return stream.str();
}

} // namespace opengl_wrapper
