#version 150 core

in vec4 color;
out vec4 out_Color;
in vec3 vn;
float angle;
int element;
uniform vec4 gradient[250];

void main(void)
{	
	 angle = abs(dot(vn, vec3(0.5,0.7,0.5))); 
	element = int(abs(angle)*250);

	out_Color = gradient[element];
}
