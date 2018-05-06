#version 130
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 color;

void main() {
  mat4 MVP = P * V * M;
  gl_Position = MVP * vec4(vertexPosition, 1);
  color = vertexColor;
}
