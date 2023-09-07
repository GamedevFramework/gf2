#version 450

layout(location = 0) out vec2 frag_texcoords;

void main() {
  frag_texcoords = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
  gl_Position = vec4(frag_texcoords * 2.0f - 1.0f, 0.0f, 1.0f);
}
