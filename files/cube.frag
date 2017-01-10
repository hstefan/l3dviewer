#version 150

in vec4 FragColor;
in vec2 TexCoord;

out vec4 outColor;

uniform float timeSinceStart;
uniform sampler2D texPepper;
uniform sampler2D texBacon;
uniform vec3 overrideColor;

void main() {
  float t = (sin(timeSinceStart * 4.0) + 1.0) * 0.5;

  // recalculate coords to generate a mirrored wavy tex in the bottom half
  vec2 coord = TexCoord;
  coord.x = coord.x + (sin(coord.y * 60 + timeSinceStart * 10.0) / 90.0);

  // sample color from both textures
  vec4 colorPepper = texture(texPepper, coord);
  vec4 colorBacon = texture(texBacon, coord);

  // mixes both textures along the animation time
  vec4 mixedColor = mix(colorPepper, colorBacon, t);

  // applies tinted color to output color
  outColor = mixedColor * FragColor * vec4(overrideColor, 1.0);
};
