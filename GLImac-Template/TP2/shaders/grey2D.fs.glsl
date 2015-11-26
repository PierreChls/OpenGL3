#version 330 core

in vec3 vFragColor;

out vec3 fFragColor;

void main()
{
	fFragColor =
		vec3(
			(vFragColor.x + vFragColor.y + vFragColor.y)/3,
			(vFragColor.x + vFragColor.y + vFragColor.y)/3,
			(vFragColor.x + vFragColor.y + vFragColor.y)/3
		);
}