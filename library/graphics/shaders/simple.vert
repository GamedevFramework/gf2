#version 450

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;

layout(set = 0, binding = 0) uniform Camera {
  mat4 u_camera;
};

layout(push_constant, std430) uniform Transform {
  mat4 u_transform;
};

layout(location = 0) out vec4 frag_color;

void main() {
  vec3 world_position = vec3(in_position, 1);
  vec3 normalized_position = world_position * mat3(u_transform) * mat3(u_camera);
  gl_Position = vec4(normalized_position.xy, 0.0, 1.0);

  frag_color = in_color;
}
