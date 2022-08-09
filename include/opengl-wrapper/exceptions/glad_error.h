#ifndef GLAD_ERROR_H
#define GLAD_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class GladError : public Exception {
  public:
    explicit GladError(const std::string &description);
};

} // namespace opengl_wrapper

#endif
