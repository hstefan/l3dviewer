#version 150

in vec4 FragColor;
in vec2 TexCoord;

out vec4 outColor;

uniform float timeSinceStart;
uniform sampler2D texPepper;
uniform sampler2D texBacon;

void main() {
  float t = (sin(timeSinceStart * 4.0) + 1.0) * 0.5;

  // recalculate coords to generate a mirrored wavy tex in the bottom half
  vec2 coord = TexCoord;
  if (coord.y > 0.5) {
    coord.y = 0.5 - (coord.y - 0.5);
    coord.x = coord.x + (sin(coord.y * 60 + timeSinceStart * 2.0) / 30.0);
  }

  // sample color from both textures
  vec4 colorPepper = texture(texPepper, coord);
  vec4 colorBacon = texture(texBacon, coord);

  // mixes both textures along the animation time
  vec4 mixedColor = mix(colorPepper, colorBacon, t);

  // applies tinted color to output color
  vec4 tintedColor = mixedColor * FragColor;
  outColor = mix(mixedColor, tintedColor, 0.6);
};
