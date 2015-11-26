#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

// vec2 complexSqr(vec2 z)
// {

// }

void main()
{
	float maxit = 1000;
	vec2 c = vFragPosition.xy;
	vec2 z = c;

	fFragColor = vec3(0, 0, 0);

	for(float i = 0; i < maxit; i += 1.0)
	{
		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
		if (dot(z, z) > 4.0)
		{
			fFragColor = vec3(1, 1, 1);
			break;
		}
	}
}