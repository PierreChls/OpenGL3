#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vVertexPosition;

mat3 translate(float tx, float ty){
	mat3 M = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
	return M;
}
mat3 scale(float sx, float sy){
	mat3 M = mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
	return M;
}

mat3 rotate(float b){
  float a = radians(b);
	mat3 M = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));
	return M;
}

void main() {
  
  vVertexPosition = aVertexPosition;
  vFragColor = aVertexColor;
  gl_Position = vec4(aVertexPosition, 0, 1);

  mat3 M_translate = translate(0.5, 0);
  mat3 M_scale = scale(4, 4);
  mat3 M_rotate = rotate(-30);
  mat3 M_rotate_triangle = rotate(30);

  //Doublez la taille du triangle sur l'axe x et réduisez la de moitié sur l'axe y
  //gl_Position = vec4(aVertexPosition.x*2, aVertexPosition.y/2, 0, 1);
  
  //Doublez la taille du triangle
  //gl_Position = vec4(vec2(aVertexPosition)*2 + 0.5, 0, 1);

  //Translatez le triangle selon le vecteur (0.5,0.5)
  //gl_Position = vec4(aVertexPosition + 0.5, 0, 1);

  //Transalation via la matrice de translation M
  //gl_Position = vec4((M_translate * vec3(aVertexPosition, 1)).xy, 0, 1);

  //Transformation via la matrice de scale M
  //gl_Position = vec4((M_scale * vec3(aVertexPosition, 1)).xy, 0, 1);

  //Rotation via la matrice de rotation M
  //gl_Position = vec4((M_rotate * vec3(aVertexPosition, 1)).xy, 0, 1);

  //Translation puis rotation puis scale
  //gl_Position = vec4((M_translate * M_rotate * M_scale * vec3(aVertexPosition, 1)).xy, 0, 1);

  //Translation puis rotation en gardant le triangle droit
  gl_Position = vec4((M_rotate_triangle * M_translate * M_rotate * M_scale * vec3(aVertexPosition, 1)).xy, 0, 1);


};