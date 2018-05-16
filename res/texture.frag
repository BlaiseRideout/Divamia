#version 130

in vec2 tcoord;

uniform sampler2D tex;

out vec4 fragColor;

void main(void) {
	fragColor = texture2D(tex, vec2(tcoord.x, tcoord.y));
}
