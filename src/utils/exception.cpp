#include "game-engine/utils/exception.h"

#include <boost/log/trivial.hpp>
#include <boost/stacktrace.hpp>

namespace game_engine {

exception_t::exception_t(const std::string &description) : std::runtime_error(description) {
    BOOST_LOG_TRIVIAL(warning) << description;
    BOOST_LOG_TRIVIAL(warning) << boost::stacktrace::stacktrace();
}

} // namespace game_engine
