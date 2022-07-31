#include "unregistered_window_error.h"

namespace opengl_wrapper {

UnregisteredWindowError::UnregisteredWindowError(std::string description)
    : Exception(description) {}

} // namespace opengl_wrapper
