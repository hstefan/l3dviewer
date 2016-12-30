#version 150

uniform vec3 triangleColor;

in float t;

out vec4 outColor;

void main() {
	outColor = vec4(t * triangleColor, 1.0);
};