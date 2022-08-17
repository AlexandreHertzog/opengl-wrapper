#include "opengl-wrapper/utils/glad_error.h"

namespace opengl_wrapper {

glad_error::glad_error(const std::string &description) : exception(description) {
}

} // namespace opengl_wrapper
