#include "texture_controller.h"

#include "image.h"
#include "texture.h"
#include <cassert>

namespace opengl_wrapper {

texture &texture_controller::operator[](int index) {
    assert(index >= 0);
    assert(index < m_textures.size());
    return m_textures[index];
}

int texture_controller::add_texture(const std::filesystem::path &path) {
    texture t(GL_TEXTURE_2D);
    t.bind();

    t.set_parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    t.set_parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    t.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    t.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image i(path);
    t.set_image(0, GL_RGB, i.get_width(), i.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, i.get_data());
    t.generate_mipmap();

    m_textures.emplace_back(std::move(t));

    return m_textures.size() - 1;
}

} // namespace opengl_wrapper