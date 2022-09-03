#version 330 core
out vec4 frag_out_color;

in vec2 vert_out_tex_coord;
in vec3 vert_out_normals;
in vec3 vert_out_position;

#define LIGHT_AMBIENT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2

struct light_t {
    int type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
    float cutoff_begin;
    float cutoff_end;
};

struct material_t {
    vec3 ambient;
    bool has_diffuse;
    sampler2D diffuse;
    bool has_specular;
    sampler2D specular;
    float shininess;
    sampler2D texture1;
    sampler2D texture2;
    float texture_mix;
};

uniform vec3 uniform_view_pos;
uniform material_t uniform_material;
uniform light_t uniform_light;

vec3 build_light_ambient(float attenuation);
vec3 build_light_diffuse(vec3 normals, vec3 light_direction, float attenuation);
vec3 build_light_specular(vec3 normals, vec3 light_direction, float attenuation);

vec3 calculate_light_direction();
float calculate_attenuation();
float calculate_light_intensity(vec3 light_direction);

void main()
{
    vec4 object_color = mix(texture(uniform_material.texture1, vert_out_tex_coord),
                            texture(uniform_material.texture2, vert_out_tex_coord),
                            uniform_material.texture_mix);

    vec3 light_direction = calculate_light_direction();
    float light_intensity = calculate_light_intensity(light_direction);

    if (light_intensity > 0.0) {
        vec3 normals = normalize(vert_out_normals);
        float light_attenuation = calculate_attenuation() * light_intensity;
        vec3 ambient_light = build_light_ambient(light_attenuation);
        vec3 diffuse_light = build_light_diffuse(normals, light_direction, light_attenuation);
        vec3 specular_light = build_light_specular(normals, light_direction, light_attenuation);
        frag_out_color = vec4(ambient_light + diffuse_light + specular_light, 1.0) * object_color;
    }
}

vec3 build_light_ambient(float attenuation) {
    return uniform_light.ambient * uniform_material.ambient * attenuation;
}

vec3 build_light_diffuse(vec3 normals, vec3 light_direction, float attenuation) {
    float absolute = max(dot(normals, light_direction), 0.0);
    vec3 ret = uniform_light.diffuse * absolute;
    if (uniform_material.has_diffuse) {
        ret *= vec3(texture(uniform_material.diffuse, vert_out_tex_coord));
    }
    return ret * attenuation;
}

vec3 build_light_specular(vec3 normals, vec3 light_direction, float attenuation) {
    vec3 view_direction = normalize(uniform_view_pos - vert_out_position);
    vec3 reflect_direction = reflect(-light_direction, normals);

    float absolute = pow(max(dot(view_direction, reflect_direction), 0.0), uniform_material.shininess);
    vec3 ret = uniform_light.specular * absolute;
    if (uniform_material.has_specular) {
        ret *= vec3(texture(uniform_material.specular, vert_out_tex_coord));
    }
    return ret * attenuation;
}

vec3 calculate_light_direction() {
    if (uniform_light.type == LIGHT_DIRECTIONAL) {
        return normalize(-uniform_light.direction);
    }
    return normalize(uniform_light.position - vert_out_position);
}

float calculate_attenuation() {
    float distance = length(uniform_light.position - vert_out_position);
    return 1.0 / (uniform_light.attenuation_constant +
                  uniform_light.attenuation_linear * distance +
                  uniform_light.attenuation_quadratic * (distance * distance));
}

float calculate_light_intensity(vec3 direction) {
    if (uniform_light.type != LIGHT_SPOT) {
        return 1.0;
    }

    float light_angle = dot(direction, normalize(-uniform_light.direction));
    float smooth_cutoff = uniform_light.cutoff_begin - uniform_light.cutoff_end;
    return clamp((light_angle - uniform_light.cutoff_end) / smooth_cutoff, 0.0, 1.0);
}
