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
     s.x=in_Position.x;     
     s.y=in_Position.y;     
     s.z=in_Position.z;     
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(s, 1.0);
     pass_Color.x = s.x;
     pass_Color.y = s.y;
     pass_Color.z = s.z;
}
