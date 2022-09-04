#include "opengl-wrapper/data_types/image.h"

#define STB_IMAGE_IMPLEMENTATION

#include "opengl-wrapper/utils/exception.h"
#include "stb_image.h"

namespace opengl_wrapper {

image_t::image_t(const std::filesystem::path &path) {
    stbi_set_flip_vertically_on_load(1);
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_num_channels, 0);
    if (nullptr == m_data) {
        throw exception_t("failed to open image: " + path.string());
    }
}

image_t::image_t(image_t &&other) noexcept
    : m_width(other.m_width), m_height(other.m_height), m_num_channels(other.m_num_channels), m_data(other.m_data) {

    other.m_width = 0;
    other.m_height = 0;
    other.m_num_channels = 0;
    other.m_data = nullptr;
}

image_t::~image_t() {
    stbi_image_free(m_data);
}

image_t &image_t::operator=(image_t &&other) noexcept {
    m_width = other.m_width;
    other.m_width = 0;

    m_height = other.m_height;
    other.m_height = 0;

    m_num_channels = other.m_num_channels;
    other.m_num_channels = 0;

    m_data = other.m_data;
    other.m_data = nullptr;
    return *this;
}

int image_t::get_width() const {
    return m_width;
}

int image_t::get_height() const {
    return m_height;
}

const unsigned char *image_t::get_data() const {
    return m_data;
}

bool image_t::has_alpha() const {
    return m_num_channels == 4;
}
} // namespace opengl_wrapper