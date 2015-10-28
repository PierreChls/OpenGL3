#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexCoord;

out vec2 vVertexPosition;
out vec3 vFragColor;

uniform mat3 uModelMatrix;


void main() {

  //vVertexCoord = aVertexCoord;
  vVertexPosition = aVertexPosition;

  vec3 FragRed = vec3(1, 0, 0);
  vFragColor = FragRed;

  //gl_Position = vec4(aVertexPosition, 0, 1);
  gl_Position = vec4(uModelMatrix * vec3(aVertexPosition, 1), 1);

};