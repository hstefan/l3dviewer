#version 150

uniform float timeSinceStart;

in vec2 position;
in vec3 color;
in vec2 texCoord;

out float t;
out vec3 fragColor;
out vec2 texCoord_;

void main() {
  t = (sin(timeSinceStart * 4.0) + 1.0) * 0.5;
  fragColor = color;
  gl_Position = vec4(position.x, position.y, 0.0, 1.0);
  texCoord_ = texCoord;
};