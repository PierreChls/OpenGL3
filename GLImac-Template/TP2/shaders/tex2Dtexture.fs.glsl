#version 330

in vec2 vTexture;

uniform vec3 uColor;
uniform sampler2D uTexture;

out vec3 fFragColor;




void main() {
    //fFragColor = vec3(1,0,0);

    vec4 color = texture(uTexture, vTexture);
    fFragColor = color.rgb;
    //fFragColor = vec3(vTexture,1);
}
