#version 130

in vec3 color;

out vec4 vertColor;

void main() {
  vertColor = vec4(color, 1);
}
