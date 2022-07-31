#ifndef OPGENGL_WRAPPER_UNREGISTERED_WINDOW_ERROR_H
#define OPGENGL_WRAPPER_UNREGISTERED_WINDOW_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class UnregisteredWindowError : public Exception {
  public:
    UnregisteredWindowError(std::string description);
};

} // namespace opengl_wrapper

#endif
