#version 150

in float t;
in vec3 fragColor;
in vec2 texCoord_;

out vec4 outColor;

uniform sampler2D texPepper;
uniform sampler2D texBacon;

void main() {
  vec3 invColor = vec3(1, 1, 1) - fragColor;
  vec4 colorPepper = texture(texPepper, texCoord_);
  vec4 colorBacon = texture(texBacon, texCoord_);
  outColor = mix(colorPepper, colorBacon, t) * vec4(t * invColor, 1.0);
};
