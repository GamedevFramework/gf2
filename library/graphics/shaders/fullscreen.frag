#version 450

layout(set = 2, binding = 0) uniform sampler2D u_texture;

layout(location = 0) in vec2 frag_texcoords;

layout(location = 0) out vec4 out_color;

void main() {
  out_color = texture(u_texture, frag_texcoords);
}
