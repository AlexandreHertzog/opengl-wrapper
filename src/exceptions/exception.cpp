#include "exception.h"

#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>

namespace opengl_wrapper {

exception::exception(const std::string &description) : std::runtime_error(description) {
    BOOST_LOG_TRIVIAL(warning) << description;
    BOOST_LOG_TRIVIAL(warning) << boost::stacktrace::stacktrace();
}

} // namespace opengl_wrapper
