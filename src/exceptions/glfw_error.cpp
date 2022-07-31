#include "glfw_error.h"

namespace opengl_wrapper {

GlfwError::GlfwError(std::string description) : Exception(description) {}

} // namespace opengl_wrapper
