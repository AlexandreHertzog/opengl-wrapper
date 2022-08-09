#include "formatter.h"

#include <iomanip>
#include <sstream>

namespace opengl_wrapper {
std::string Formatter::formatGlfwErrorCode(int error_code) {
    std::stringstream stream;
    stream << "0x" << std::internal << std::hex << std::setw(8)
           << std::setfill('0') << error_code;
    return stream.str();
}

} // namespace opengl_wrapper
