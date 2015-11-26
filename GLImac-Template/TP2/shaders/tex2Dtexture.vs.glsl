#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

uniform mat3 uModelMatrix;

out vec2 vTexture;


void main() {
    vTexture = aVertexTexture;
    gl_Position = vec4((uModelMatrix * vec3(aVertexPosition, 1)).xy, 0, 1);
}

