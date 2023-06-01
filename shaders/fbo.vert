#version 150

uniform mat4 projmat;
uniform mat4 modelmat;
in vec3 position;
in vec2 texcoord;
out vec2 coord;

void main() {
	coord.s = texcoord.s;
	coord.t = texcoord.t;
	gl_Position=vec4(position,1.0);
}

