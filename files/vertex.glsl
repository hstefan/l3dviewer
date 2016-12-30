#version 150

uniform float timeSinceStart;

in vec2 position;

out float t;

void main() {
  t = (sin(timeSinceStart * 4.0) + 1.0) * 0.5;
  gl_Position = vec4(position, 0.0, 1.0);
};