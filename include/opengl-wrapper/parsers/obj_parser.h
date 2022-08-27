#pragma once

#include "opengl-wrapper/data_types/face_vertex_indices.h"
#include "opengl-wrapper/utils/utils.h"
#include <filesystem>
#include <fstream>
#include <vector>

namespace opengl_wrapper {

class obj_parser {
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

    explicit obj_parser(const std::filesystem::path &path = "");
    void open(const std::filesystem::path &path);

    line_type_t line_type();
    [[nodiscard]] bool is_good() const;
    [[nodiscard]] const std::string &get_header() const;

    template <class TYPE> void get_line(TYPE &out) {
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

template <> void obj_parser::get_line(bool &out);

} // namespace opengl_wrapper
