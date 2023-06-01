#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in vec4 in_Color;
out vec3 olor;

vec3 s;
out vec2 tex;
out float z;
void main(void)
{
     s.x = in_Position.x;
     s.y = in_Position.y;
     s.z = in_Position.z;
     tex = s.xy;
     gl_Position =projectionMatrix* modelMatrix*viewMatrix*vec4(s,1.0f);
     z = s.z;
     olor.r = in_Color.r;
     olor.g = in_Color.g;
     olor.b = in_Color.b;     
}
