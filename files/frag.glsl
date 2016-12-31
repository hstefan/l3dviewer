#version 150

in float t;
in vec3 fragColor;

out vec4 outColor;

void main() {
	outColor = vec4(t * fragColor, 1.0);
};
