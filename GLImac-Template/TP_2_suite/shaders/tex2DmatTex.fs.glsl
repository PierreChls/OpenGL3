#version 330 core

in vec3 vFragColor;
in vec2 vVertexCoord;

uniform vec3 uModelColor;
uniform sampler2D uTexture;

out vec3 fFragColor;

void main() {
  //fFragColor = uModelColor;
  vec4 idtexture = texture(uTexture, vVertexCoord);
   fFragColor = idtexture.rgb;

  //fFragTexture = texture(uTexture, vVertexCoord);
};