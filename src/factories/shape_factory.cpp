#include "factories/shape_factory.h"
#include "utils/configuration.h"

namespace {

constexpr auto default_ambient = 0.2F;
constexpr auto default_shininess = 32.0F;
constexpr auto default_texture_mix = 0.8F;

} // namespace

namespace game_engine {

shape_factory_t::shape_factory_t(opengl_cpp::gl_t &gl, texture_factory_t &texture_factory)
    : m_gl(gl), m_texture_factory(texture_factory) {
}

shape_pointer_t shape_factory_t::build_cube(texture_pointer_t &base_texture) {
    constexpr auto rotation_angle = 45.0F;
    constexpr auto scale = 0.5F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/cube.obj"));

    transform_t t;
    t.m_translation = {0.0F, 0.0F, -1.0F};
    t.m_rotation_angle = rotation_angle;
    t.m_rotation_axis = {0.0F, 1.0F, 0.0F};
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(default_ambient);
    mat.m_shininess = default_shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/blue.png", configuration::texture_layer_2);
    mat.m_diffuse = m_texture_factory.build_texture("./textures/diffuse.png", configuration::texture_diffuse);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_plane(texture_pointer_t &base_texture) {
    constexpr auto scale = 10.0F;
    constexpr glm::vec3 position = {0.0F, -0.5F, 0.0F};

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/plane.obj"));

    transform_t t;
    t.m_translation = position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(default_ambient);
    mat.m_shininess = default_shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/orange.png", configuration::texture_layer_2);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_sphere(texture_pointer_t &base_texture) {
    constexpr auto scale = 0.6F;
    constexpr auto ambient = 0.1F;
    constexpr auto shininess = 32.0F;
    constexpr glm::vec3 position = {1.5F, 0.0F, -1.0F};

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t;
    t.m_translation = position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = glm::vec3(ambient);
    mat.m_shininess = shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/red.png", configuration::texture_layer_2);
    mat.m_diffuse = m_texture_factory.build_texture("./textures/diffuse.png", configuration::texture_diffuse);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_torus(texture_pointer_t &base_texture) {
    constexpr auto scale = 0.6F;
    constexpr auto shininess = 2.0F;
    constexpr auto rotation_angle = 45.0F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/torus.obj"));

    transform_t t;
    t.m_translation = {-1.0F, 0.0F, -1.0F};
    t.m_rotation_axis = {0.0F, 0.0F, 1.0F};
    t.m_rotation_angle = rotation_angle;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_ambient = {1.0F, 1.0F, 1.0F};
    mat.m_shininess = shininess;
    mat.m_texture_mix = default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/green.png", configuration::texture_layer_2);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_light_shape(const light_pointer_t &light, texture_pointer_t &base_texture) {
    constexpr auto scale = 0.1F;

    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t;
    t.m_translation = light->m_position;
    t.m_scale = glm::vec3(scale);
    ret->set_transform(t);

    material_t mat;
    mat.m_texture1 = base_texture;
    ret->set_material(std::move(mat));

    return ret;
}

} // namespace game_engine
