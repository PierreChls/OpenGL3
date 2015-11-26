#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

void main()
{
	float alpha = 2;
	float beta = 80;

	float attenuation = alpha * exp(-beta * pow(length(vFragPosition), 2));
	// float attenuation = length(fract(5.0 * vFragPosition));
	// float attenuation = length(abs(fract(5.0 * vFragPosition) * 2.0 - 1.0));
	// float attenuation = mod(floor(10.0 * vFragPosition.x) + floor(10.0 * vFragPosition.y), 2.0);
	// float attenuation = smoothstep(0.3, 0.32, length(fract(5.0 * vFragPosition) - 0.5));
	// float attenuation = smoothstep(0.4, 0.5, max(abs(fract(8.0 * vFragPosition.x - 0.5 * mod(floor(8.0 * vFragPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vFragPosition.y) - 0.5)));

	fFragColor = vFragColor * attenuation;
}