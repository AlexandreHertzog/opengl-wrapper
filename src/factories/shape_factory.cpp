#include "factories/shape_factory.h"
#include "utils/configuration.h"

namespace game_engine {

shape_factory_t::shape_factory_t(opengl_cpp::gl_t &gl, texture_factory_t &texture_factory)
    : m_gl(gl), m_texture_factory(texture_factory) {
}

shape_pointer_t shape_factory_t::build_cube() {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/cube.obj"));
    ret->set_transform(configuration::object_cube_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_default_ambient;
    mat.m_shininess = configuration::material_default_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = m_texture_factory.get_base_texture();
    mat.m_texture2 = m_texture_factory.build_blue_texture();
    mat.m_diffuse = m_texture_factory.build_diffuse_texture();
    mat.m_specular = m_texture_factory.build_specular_texture();
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_plane() {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/plane.obj"));
    ret->set_transform(configuration::object_plane_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_default_ambient;
    mat.m_shininess = configuration::material_default_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = m_texture_factory.get_base_texture();
    mat.m_texture2 = m_texture_factory.build_orange_texture();
    mat.m_specular = m_texture_factory.build_specular_texture();
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_sphere() {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));
    ret->set_transform(configuration::object_sphere_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_sphere_ambient;
    mat.m_shininess = configuration::material_sphere_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = m_texture_factory.get_base_texture();
    mat.m_texture2 = m_texture_factory.build_red_texture();
    mat.m_diffuse = m_texture_factory.build_diffuse_texture();
    mat.m_specular = m_texture_factory.build_specular_texture();
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_torus() {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/torus.obj"));
    ret->set_transform(configuration::object_torus_transforms);

    material_t mat;
    mat.m_ambient = configuration::material_torus_ambient;
    mat.m_shininess = configuration::material_torus_shininess;
    mat.m_texture_mix = configuration::material_default_texture_mix;
    mat.m_texture1 = m_texture_factory.get_base_texture();
    mat.m_texture2 = m_texture_factory.build_green_texture();
    ret->set_material(std::move(mat));

    return ret;
}

shape_pointer_t shape_factory_t::build_light_shape(const light_pointer_t &light) {
    shape_pointer_t ret = std::make_shared<shape_t>(opengl_cpp::vertex_array_t(m_gl));
    ret->set_mesh(mesh_t("./objects/sphere.obj"));

    transform_t t = configuration::object_light_transforms;
    t.m_translation = light->m_position;
    ret->set_transform(t);

    material_t mat;
    mat.m_texture1 = m_texture_factory.get_base_texture();
    ret->set_material(std::move(mat));

    return ret;
}

} // namespace game_engine
