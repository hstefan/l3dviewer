#version 150

in vec2 position;
in vec3 color;
in vec2 texCoord;

out vec4 FragColor;
out vec2 TexCoord;

void main() {
  FragColor = vec4(color, 1.0);
  gl_Position = vec4(position.x, position.y, 0.0, 1.0);
  TexCoord = texCoord;
};
