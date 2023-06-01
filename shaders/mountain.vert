#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

float PI = 3.141592653;
float angle;
float ypotin;
int element;
vec4 N;
in vec3 Position;
in vec3 Normal;
out vec4 color;
out vec3 vn;

vec3 s;

void main(void)
{
     vn = normalize (Normal);
     angle = abs(dot(vn, vec3(0.0,0.5,0.0))); 
	element = int(abs(angle)*250);
     s.x = Position.x;
     s.y = Position.y;
     s.z = Position.z;
     gl_Position =projectionMatrix*modelMatrix*viewMatrix*vec4(s,1.0f);
     color = vec4(1.0,1.0,1.0,1.0);
     
     
     
}
