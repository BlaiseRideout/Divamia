#version 400
in vec3 Position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 vPosition;


void main() {
	mat4 MVP = P * V * M;
	
    vPosition = Position.xyz;
}