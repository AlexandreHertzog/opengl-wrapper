#include "glad_error.h"

namespace opengl_wrapper {

GladError::GladError(std::string description) : Exception(description) {}

} // namespace opengl_wrapper
