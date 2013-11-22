#version 330 core
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;

void main() {
  mat4 MVP = P * V * M;
  gl_Position = MVP * vertexPosition;

  fragPosition = vertexPosition;
  fragNormal = vertexNormal;
  fragUV = vertexUV;
}
