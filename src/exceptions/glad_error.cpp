#include "glad_error.h"

namespace opengl_wrapper {

glad_error::glad_error(std::string description) : exception(description) {}

} // namespace opengl_wrapper
