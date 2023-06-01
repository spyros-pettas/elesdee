#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
flat out vec3 olor;

vec3 s;

void main(void)
{
     s.x = in_Position.x;
     s.y = in_Position.y;
     s.z = in_Position.z;
     gl_Position =projectionMatrix* modelMatrix*viewMatrix*vec4(s,1.0f);
     olor.x = 0.5f;
     olor.y = 0.0;
     olor.z = 0.2f;
}
