#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

void main() {
	
  //Calcul distance
  float FragCenterDistance = distance(vVertexPosition.xy, vec2(0,0));

  float alpha = 0.5, teta = 100;
  float a = alpha * exp(-teta * (FragCenterDistance*FragCenterDistance));

  //float fractal = length(fract(vec2(5.0,5.0) * vVertexPosition.xy));
  //float fractal = mod(floor(10.0 * vVertexPosition.x) + floor(10.0 * vVertexPosition.y), 2.0);
  float fractal = smoothstep(0.4, 0.5, max(abs(fract(8.0 * vVertexPosition.x - 0.5 * mod(floor(8.0 * vVertexPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vVertexPosition.y) - 0.5)));
  //float fractal = smoothstep(0.3, 0.32, length(fract(vec2(5.0,5.0) * vVertexPosition.xy) - 0.5));

  //fFragColor = vFragColor * a;
  fFragColor = vFragColor * fractal * a;
};