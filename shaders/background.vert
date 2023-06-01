#version 150 core

in vec3 position;
out vec3 color;

void main(void)
{
     gl_Position = vec4(position,1.0f);
     if (position.y ==1.0f) {
	color.r = 0.0f;
	color.g = 0.0f;
	color.b = 0.1f;
     }
     else {
	color.r = 0.8f;
	color.g = 0.0f;
	color.b = 0.2f;
    }
     
}
