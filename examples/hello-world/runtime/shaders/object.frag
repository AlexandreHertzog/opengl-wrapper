#version 330 core
out vec4 frag_out_color;

in vec2 vert_out_tex_coord;
in vec3 vert_out_normals;
in vec3 vert_out_position;

uniform sampler2D uniform_texture1;
uniform sampler2D uniform_texture2;
uniform float uniform_texture_mix;

uniform vec3 uniform_light_color;
uniform vec3 uniform_light_pos;
uniform vec3 uniform_view_pos;
uniform float uniform_specular;
uniform float uniform_shininess;

void main()
{
    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * uniform_light_color;

    vec3 normals = normalize(vert_out_normals);
    vec3 light_direction = normalize(uniform_light_pos - vert_out_position);
    float diff = max(dot(normals, light_direction), 0.0);
    vec3 diffuse_light = diff * uniform_light_color;

    vec4 object_color = mix(texture(uniform_texture1, vert_out_tex_coord),
                            texture(uniform_texture2, vert_out_tex_coord),
                            uniform_texture_mix);

    vec3 view_direction = normalize(uniform_view_pos - vert_out_position);
    vec3 reflect_direction = reflect(-light_direction, normals);

    float specular_absolute = pow(max(dot(view_direction, reflect_direction), 0.0), uniform_shininess);
    vec3 specular_light = uniform_specular * specular_absolute * uniform_light_color;

    frag_out_color = vec4(ambient_light + diffuse_light + specular_light, 1.0) * object_color;
}