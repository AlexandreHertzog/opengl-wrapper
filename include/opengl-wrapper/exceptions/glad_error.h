#ifndef GLAD_ERROR_H
#define GLAD_ERROR_H

#include "exception.h"

namespace opengl_wrapper {

class glad_error : public exception {
  public:
    glad_error(std::string description);
};

} // namespace opengl_wrapper

#endif
