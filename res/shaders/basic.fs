#version 330

in vec2 coord;
out vec3 color;

uniform sampler2D texture;

void main() {
  color = texture(texture, coord);
}