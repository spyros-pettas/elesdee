#version 150 core

in vec3 olor;
in vec2 tex;
in float z;
out vec4 out_Color;

float fw;
vec4 fg;

void main(void)
{
    fw = sin(z);

    fg.r = sin(olor.r);
    fg.g = sin(olor.g)*0.3;
    fg.b = sin(olor.b)*0.8 + sin(olor.b);
    fg.a = 1.0;

    if (fw<0.5) {
	if (olor.r<1.7 && olor.r>1.5) {
	    fg.r = fg.r + (0.5 - fw)*sin((3.14/0.2)*(1.7-olor.r));
	    fg.g = fg.g + (0.5-fw)*sin((3.14/0.2)*(1.7-olor.g));
	    fg.b = fg.b + (0.5-fw)*sin((3.14/0.2)*(1.7-olor.b));
	    fg.a = 1.0;
	}
    }
    out_Color = vec4(fg);
}
