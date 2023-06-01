#version 150 core

flat in vec3 olor;
out vec4 out_Color;

void main(void)
{	
	out_Color = vec4(olor,1.0f);
}
