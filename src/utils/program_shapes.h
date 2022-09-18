#pragma once

#include "data_types/types.h"

namespace game_engine {

class program_shapes_t {
  public:
    using pair_t = std::pair<program_pointer_t, shape_pointer_t>;
    using vector_t = std::vector<pair_t>;

    void add_object(program_pointer_t program, shape_pointer_t shape);

    vector_t::iterator begin();
    vector_t::iterator end();

  private:
    vector_t m_values;
};

} // namespace game_engine
