#include "opengl-wrapper/utils/glfw_error.h"

namespace opengl_wrapper {

glfw_error_t::glfw_error_t(const std::string &description) : exception_t(description) {
}

} // namespace opengl_wrapper
