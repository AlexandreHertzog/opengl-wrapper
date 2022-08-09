#include "glad_error.h"

namespace opengl_wrapper {

GladError::GladError(const std::string &description) : Exception(description) {}

} // namespace opengl_wrapper
