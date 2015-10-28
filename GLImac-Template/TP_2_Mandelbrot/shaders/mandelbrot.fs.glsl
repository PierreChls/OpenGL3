#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
	vec2 z2;
	z2.x=z.x*z.x-z.y*z.y;
	z2.y=2*z.x*z.y;
	return z2;
	}


void main() {
	
	int i,NMAX;
	NMAX=1000;
	vec2 c = vFragPosition;
	vec2 z = c;
	float module;
	bool noir=true;
	for(i = 1; i < NMAX; i++){
		module = sqrt((z.x)*(z.x)+(z.y)*(z.y));
		if(module > 2){
			noir = false;
		}
		z= complexSqr(z)+c;
	}
	
	if(noir){
		fFragColor = vec3(0);
	}
	else{
		fFragColor = vec3(1);
	}
};
