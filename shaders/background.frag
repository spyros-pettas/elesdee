#version 150 core

out vec4 out_Color;
in vec3 color;
float fx;
float fy;

void main(void)
{	
	fx = gl_FragCoord.x/800;
	fy = gl_FragCoord.y/600;
	
	out_Color = vec4 (sin(3.14*fy)*1.0f,fy*0.5f, 1.0f,1.0f);
}
