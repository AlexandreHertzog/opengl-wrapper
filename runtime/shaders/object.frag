#version 330 core

#define LIGHT_DEACTIVATED 0
#define LIGHT_AMBIENT 1
#define LIGHT_DIRECTIONAL 2
#define LIGHT_SPOT 3

#define LIGHT_COUNT 10

out vec4 frag_out_color;

in vec2 vert_out_tex_coord;
in vec3 vert_out_normals;
in vec3 vert_out_position;

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

struct depth_params_t {
    bool debug;
    bool enabled;
    float near;
    float far;
};

uniform vec3 uniform_view_pos;
uniform material_t uniform_material;
uniform light_t uniform_light[LIGHT_COUNT];
uniform depth_params_t uniform_depth;

vec3 build_light_ambient(light_t light, float attenuation);
vec3 build_light_diffuse(light_t light, vec3 normals, vec3 light_direction, float attenuation);
vec3 build_light_specular(light_t light, vec3 normals, vec3 light_direction, float attenuation);

vec3 calculate_light_direction(light_t light);
float calculate_attenuation(light_t light);
float calculate_light_intensity(light_t light, vec3 light_direction);

float linearize_depth(float depth);

void main()
{
    vec4 object_color = mix(texture(uniform_material.texture1, vert_out_tex_coord),
                            texture(uniform_material.texture2, vert_out_tex_coord),
                            uniform_material.texture_mix);

    vec3 result = vec3(0.0);

    if (!uniform_depth.debug) {
        for (int i = 0; i < LIGHT_COUNT; i++) {
            if (uniform_light[i].type == LIGHT_DEACTIVATED) {
                continue;
            }

            vec3 light_direction = calculate_light_direction(uniform_light[i]);
            float light_intensity = calculate_light_intensity(uniform_light[i], light_direction);

            if (light_intensity > 0.0) {
                vec3 normals = normalize(vert_out_normals);
                float light_attenuation = calculate_attenuation(uniform_light[i]) * light_intensity;
                vec3 ambient_light = build_light_ambient(uniform_light[i], light_attenuation);
                vec3 diffuse_light = build_light_diffuse(uniform_light[i], normals, light_direction, light_attenuation);
                vec3 specular_light = build_light_specular(uniform_light[i], normals, light_direction, light_attenuation);
                result += ambient_light + diffuse_light + specular_light;
            }
        }

        if (uniform_depth.enabled) {
            result *= linearize_depth(gl_FragCoord.z);
        }

        frag_out_color = vec4(result, 1.0) * object_color;
    } else {
        float d = linearize_depth(gl_FragCoord.z);
        frag_out_color = vec4(vec3(d), 1.0);
    }
}

vec3 build_light_ambient(light_t light, float attenuation) {
    return light.ambient * uniform_material.ambient * attenuation;
}

vec3 build_light_diffuse(light_t light, vec3 normals, vec3 light_direction, float attenuation) {
    float absolute = max(dot(normals, light_direction), 0.0);
    vec3 ret = light.diffuse * absolute;
    if (uniform_material.has_diffuse) {
        ret *= vec3(texture(uniform_material.diffuse, vert_out_tex_coord));
    }
    return ret * attenuation;
}

vec3 build_light_specular(light_t light, vec3 normals, vec3 light_direction, float attenuation) {
    vec3 view_direction = normalize(uniform_view_pos - vert_out_position);
    vec3 reflect_direction = reflect(-light_direction, normals);

    float absolute = pow(max(dot(view_direction, reflect_direction), 0.0), uniform_material.shininess);
    vec3 ret = light.specular * absolute;
    if (uniform_material.has_specular) {
        ret *= vec3(texture(uniform_material.specular, vert_out_tex_coord));
    }
    return ret * attenuation;
}

vec3 calculate_light_direction(light_t light) {
    if (light.type == LIGHT_DIRECTIONAL) {
        return normalize(-light.direction);
    }
    return normalize(light.position - vert_out_position);
}

float calculate_attenuation(light_t light) {
    float distance = length(light.position - vert_out_position);
    return 1.0 / (light.attenuation_constant +
                  light.attenuation_linear * distance +
                  light.attenuation_quadratic * (distance * distance));
}

float calculate_light_intensity(light_t light, vec3 direction) {
    if (light.type != LIGHT_SPOT) {
        return 1.0;
    }

    float light_angle = dot(direction, normalize(-light.direction));
    float smooth_cutoff = light.cutoff_begin - light.cutoff_end;
    return clamp((light_angle - light.cutoff_end) / smooth_cutoff, 0.0, 1.0);
}

float linearize_depth(float d) {
    float n = uniform_depth.near;
    float f = uniform_depth.far;

    float z = d * 2.0 - 1.0;
    return 1 - ((2.0 * n * f) / (f + n - z * (f - n))) / f;
}
