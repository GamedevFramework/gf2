#version 450

layout(set = 2, binding = 0) uniform sampler2D u_texture;

layout(std140, set = 2, binding = 1) readonly buffer Text {
  vec4 u_outline_color;
  float u_outline_thickness;
};

layout(location = 0) in vec2 frag_texcoords;
layout(location = 1) in vec4 frag_color;

layout(location = 0) out vec4 out_color;

const float width = 0.25 / 8;

void main() {
  float outline_limit = clamp((1 - u_outline_thickness) / 2, width, 0.5);
  float sdistance = texture(u_texture, frag_texcoords).r;
  float outline_factor = smoothstep(0.5 - width, 0.5 + width, sdistance);
  vec4 color = mix(u_outline_color, frag_color, outline_factor);
  float alpha = smoothstep(outline_limit - width, outline_limit + width, sdistance);
  out_color = vec4(color.rgb, color.a * alpha);
}
