#include "exception.h"

#define BOOST_STACKTRACE_USE_ADDR2LINE

#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>
#include <iostream>

namespace opengl_wrapper {

Exception::Exception(std::string description)
    : std::runtime_error(description) {
    BOOST_LOG_TRIVIAL(warning) << boost::stacktrace::stacktrace();
}

} // namespace opengl_wrapper
