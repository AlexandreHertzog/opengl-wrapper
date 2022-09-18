#include "program_shapes.h"

#include "factories/program_factory.h"

namespace game_engine {

void program_shapes_t::add_object(program_pointer_t program, shape_pointer_t shape) {
    auto find = std::find_if(m_values.begin(), m_values.end(), [&](pair_t &v) {
        return v.second == shape;
    });
    if (m_values.end() == find) {
        m_values.emplace_back(std::make_pair(std::move(program), std::move(shape)));
    }
}

std::vector<program_shapes_t::pair_t>::iterator program_shapes_t::begin() {
    return m_values.begin();
}

std::vector<program_shapes_t::pair_t>::iterator program_shapes_t::end() {
    return m_values.end();
}
} // namespace game_engine
