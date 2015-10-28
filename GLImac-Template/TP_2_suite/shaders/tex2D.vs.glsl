#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexCoord;

out vec2 vVertexPosition;
out vec3 vFragColor;


mat3 rotate(float b){
  float a = radians(b);
	mat3 M = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));
	return M;
}

uniform float uTime;


void main() {

  mat3 M_rotate = rotate(uTime);

  //vVertexCoord = aVertexCoord;
  vVertexPosition = aVertexPosition;

  vec3 FragRed = vec3(1, 0, 0);
  vFragColor = FragRed;

  //gl_Position = vec4(aVertexPosition, 0, 1);
  gl_Position = vec4(M_rotate * vec3(aVertexPosition, 0), 1);

};