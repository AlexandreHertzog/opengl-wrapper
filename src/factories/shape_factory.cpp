#include "factories/shape_factory.h"
#include "utils/configuration.h"

namespace game_engine {

shape_factory_t::shape_factory_t(opengl_cpp::gl_t &gl, texture_factory_t &texture_factory)
    : m_gl(gl), m_texture_factory(texture_factory) {
}

shape_pointer_t shape_factory_t::build_cube(texture_pointer_t &base_texture) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/cube.obj"));
    ret->set_transform(configuration::object_cube_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_default_ambient;
    mat.m_shininess = configuration::material_default_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/blue.png", configuration::texture_layer_2);
    mat.m_diffuse = m_texture_factory.build_texture("./textures/diffuse.png", configuration::texture_diffuse);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_plane(texture_pointer_t &base_texture) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/plane.obj"));
    ret->set_transform(configuration::object_plane_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_default_ambient;
    mat.m_shininess = configuration::material_default_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/orange.png", configuration::texture_layer_2);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_sphere(texture_pointer_t &base_texture) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));
    ret->set_transform(configuration::object_sphere_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_sphere_ambient;
    mat.m_shininess = configuration::material_sphere_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/red.png", configuration::texture_layer_2);
    mat.m_diffuse = m_texture_factory.build_texture("./textures/diffuse.png", configuration::texture_diffuse);
    mat.m_specular = m_texture_factory.build_texture("./textures/specular.png", configuration::texture_specular);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_torus(texture_pointer_t &base_texture) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/torus.obj"));
    ret->set_transform(configuration::object_torus_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_torus_ambient;
    mat.m_shininess = configuration::material_torus_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = base_texture;
    mat.m_texture2 = m_texture_factory.build_texture("./textures/green.png", configuration::texture_layer_2);
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_light_shape(const light_pointer_t &light, texture_pointer_t &base_texture) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t = configuration::object_light_transforms;
    t.m_translation = light->m_position;
    ret->set_transform(t);

    material_t mat;
    mat.m_texture1 = base_texture;
    ret->set_material(std::move(mat));

    return ret;
}

} // namespace game_engine
