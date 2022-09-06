#pragma once

#include "game-engine/data_types/face.h"
#include "game-engine/utils/utils.h"
#include <filesystem>
#include <fstream>
#include <vector>

namespace game_engine {

class obj_parser_t {
  public:
    enum class line_type_t {
        undefined = 0,
        ignored,
        material_library,
        object_name,
        vertex,
        texture_coordinate,
        vertex_normal,
        used_material,
        smoothing,
        face
    };

    explicit obj_parser_t(const std::filesystem::path &path = "");
    void open(const std::filesystem::path &path);

    line_type_t line_type();
    [[nodiscard]] bool is_good() const;
    [[nodiscard]] const std::string &get_header() const;

    template <class type_t> void get_line(type_t &out) {
        m_line_stream >> out;
        prepare_next_line();
    }

    void get_line();

  private:
    std::ifstream m_file;
    std::string m_current_line;
    std::stringstream m_line_stream;
    bool m_is_good{};
    std::string m_header;

    void prepare_next_line();
};

template <> void obj_parser_t::get_line(bool &out);

} // namespace game_engine
