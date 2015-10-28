#version 330 core

in vec3 vFragColor;
uniform vec3 uModelColor;

out vec3 fFragColor;

void main() {
  fFragColor = uModelColor;
};