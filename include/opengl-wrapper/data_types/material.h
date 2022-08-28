#pragma once

namespace opengl_wrapper {

class material {
  public:
    explicit material(float specular = {0.5F}, float shininess = {32.0F});
    material &specular(float v);
    [[nodiscard]] float specular() const;

    material &shininess(float v);
    [[nodiscard]] float shininess() const;

  private:
    float m_specular;
    float m_shininess;
};

} // namespace opengl_wrapper
