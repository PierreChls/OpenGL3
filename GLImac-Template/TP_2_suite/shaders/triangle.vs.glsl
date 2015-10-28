#version 330

//Ces deux lignes déclarent les attributs de vertex du shader
//la partie location = X indique l'index associé à chaque attribut. 

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec3 aVertexColor;

out vec3 vColor;

void main() {
    vColor = aVertexColor;
    gl_Position = vec4(aVertexPosition, 0, 1);
}
