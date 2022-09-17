#include "obj_parser.h"

namespace game_engine {
obj_parser_t::obj_parser_t(const std::filesystem::path &path) {
    if (!path.empty()) {
        open(path);
    }

    m_line_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
}

void obj_parser_t::open(const std::filesystem::path &path) {
    m_file.open(path);
    if (!m_file) {
        throw exception_t("Failed to open wavefront object file: " + path.string());
    }
    prepare_next_line();
}

bool obj_parser_t::is_good() const {
    return m_is_good;
}

const std::string &obj_parser_t::get_header() const {
    return m_header;
}

obj_parser_t::line_type_t obj_parser_t::line_type() {
    m_line_stream.seekg(0);
    m_line_stream >> m_header;

    line_type_t ret = line_type_t::undefined;
    if ("#" == m_header) {
        ret = line_type_t::ignored;
    } else if ("mtllib" == m_header) {
        ret = line_type_t::material_library;
    } else if ("o" == m_header) {
        ret = line_type_t::object_name;
    } else if ("v" == m_header) {
        ret = line_type_t::vertex;
    } else if ("vt" == m_header) {
        ret = line_type_t::texture_coordinate;
    } else if ("vn" == m_header) {
        ret = line_type_t::vertex_normal;
    } else if ("usemtl" == m_header) {
        ret = line_type_t::used_material;
    } else if ("s" == m_header) {
        ret = line_type_t::smoothing;
    } else if ("f" == m_header) {
        ret = line_type_t::face;
    }

    return ret;
}

void obj_parser_t::prepare_next_line() {
    m_is_good = std::getline(m_file, m_current_line).good();
    if (m_is_good) {
        m_line_stream.str(m_current_line);
    }
}

template <> void obj_parser_t::get_line(bool &out) {
    std::string v;
    get_line(v);
    out = "on" == v;
}

void obj_parser_t::get_line() {
    prepare_next_line();
}

} // namespace game_engine