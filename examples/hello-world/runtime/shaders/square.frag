#version 330 core
out vec4 frag_out_color;

in vec3 vert_out_color;
in vec2 vert_out_tex_coord;

uniform sampler2D uniform_texture1;
uniform sampler2D uniform_texture2;
uniform float uniform_texture_mix;

void main()
{
    frag_out_color = mix(texture(uniform_texture1, vert_out_tex_coord),
                         texture(uniform_texture2, vert_out_tex_coord),
                         uniform_texture_mix);
}