#version 130
in vec3 vertexPosition;
in vec2 vertexUV;

uniform mat4 P;

out vec2 tcoord;

void main(void) {
	gl_Position = P * vec4(vertexPosition, 1.0);
	tcoord = vertexUV;
}
