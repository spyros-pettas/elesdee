#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in float time;
in vec4 in_Color;

out vec3 pass_Color;

vec3 s;

void main(void)
{
     s.x=in_Position.x*8.0f;     
     s.y=in_Position.y*8.0f;     
     s.z=in_Position.z*8.0f;     
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(s, 1.0);
     pass_Color.x = in_Color.r;
     pass_Color.y = in_Color.g;
     pass_Color.z = in_Color.b;
}
