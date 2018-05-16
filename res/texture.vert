#version 130
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec3 vertexPosition;
in vec2 vertexUV;

out vec2 tcoord;

void main(void) {
  mat4 MVP = P * V * M;
  gl_Position = MVP * vec4(vertexPosition, 1);
  tcoord = vertexUV;
}
