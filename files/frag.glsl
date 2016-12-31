#version 150

in float t;
in vec3 fragColor;

out vec4 outColor;

void main() {
	vec3 invColor = vec3(1, 1, 1) - fragColor;
	outColor = vec4(t * invColor, 1.0);
};
