#version 450

layout(set = 1, binding = 0) uniform sampler2D u_texture;

layout(location = 0) in vec2 frag_texcoords;
layout(location = 1) in vec4 frag_color;

layout(location = 0) out vec4 out_color;

const float width = 0.25 / 8;

void main() {
  vec4 color = texture(u_texture, frag_texcoords);
  float alpha = smoothstep(0.5 - width, 0.5 + width, color.r);
  out_color = vec4(frag_color.xyz, frag_color.a * alpha);
}
