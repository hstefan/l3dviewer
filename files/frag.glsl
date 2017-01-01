#version 150

in float t;
in vec3 fragColor;
in vec2 texCoord_;

out vec4 outColor;

uniform sampler2D tex;

void main() {
  vec3 invColor = vec3(1, 1, 1) - fragColor;
  outColor = texture(tex, texCoord_) * vec4(t * invColor, 1.0);
};
