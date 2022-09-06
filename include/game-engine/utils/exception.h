#pragma once

#include <stdexcept>

namespace game_engine {

class exception_t : public std::runtime_error {
  public:
    explicit exception_t(const std::string &description);
};

} // namespace game_engine
