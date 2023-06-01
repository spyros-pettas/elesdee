#version 150 core

uniform sampler2D texture;
in vec2 coord;
out vec4 color;

void main() {
	color=texture2D(texture,coord);
}

