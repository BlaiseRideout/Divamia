#version 130
in vec2 tcoord;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main(void) {
    fragColor = vec4(color, texture2D(tex, vec2(tcoord.x, tcoord.y)).a);
}
