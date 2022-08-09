#include "glfw_error.h"

namespace opengl_wrapper {

GlfwError::GlfwError(const std::string &description) : Exception(description) {}

} // namespace opengl_wrapper
