#version 330 core

in vec3 vFragColor;
in vec2 vVertexCoord;

uniform vec3 uModelColor;

out vec3 fFragColor;

sampler2D uTexture;

void main() {
  fFragColor = uModelColor;
  texture(uTexture, vVertexCoord);
};