#version 330 core
out vec4 frag_out_color;

in vec2 vert_out_tex_coord;
in vec3 vert_out_normals;
in vec3 vert_out_position;

struct light_t {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct material_t {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D uniform_texture1;
uniform sampler2D uniform_texture2;
uniform float uniform_texture_mix;
uniform vec3 uniform_view_pos;
uniform material_t uniform_material;
uniform light_t uniform_light;

void main()
{
    vec3 ambient_light = uniform_light.ambient * uniform_material.ambient;

    vec3 normals = normalize(vert_out_normals);
    vec3 light_direction = normalize(uniform_light.position - vert_out_position);
    float diffuse_absolute = max(dot(normals, light_direction), 0.0);
    vec3 diffuse_light = uniform_light.diffuse * (diffuse_absolute * uniform_material.diffuse);

    vec4 object_color = mix(texture(uniform_texture1, vert_out_tex_coord),
                            texture(uniform_texture2, vert_out_tex_coord),
                            uniform_texture_mix);

    vec3 view_direction = normalize(uniform_view_pos - vert_out_position);
    vec3 reflect_direction = reflect(-light_direction, normals);

    float specular_absolute = pow(max(dot(view_direction, reflect_direction), 0.0), uniform_material.shininess);
    vec3 specular_light = uniform_light.specular * (specular_absolute * uniform_material.specular);

    frag_out_color = vec4(ambient_light + diffuse_light + specular_light, 1.0) * object_color;
}