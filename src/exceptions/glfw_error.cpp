#include "glfw_error.h"

namespace opengl_wrapper {

glfw_error::glfw_error(std::string description) : exception(description) {}

} // namespace opengl_wrapper
