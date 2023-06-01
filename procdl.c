/*
 * =====================================================================================
 *
 *       Filename:  procdl.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/08/2011 03:47:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "GL/glfw.h"
#include <math.h>

#define Pi 3.141592

GLfloat smooth_step (GLfloat a, GLfloat b, GLfloat x)
{
    if (x<a)
	return 0;
    if (x>=b)
	return 1;
    x = (x-a)/(b-a);
    return (x*x * (3 - 2*x));
}

void tangent (GLfloat* cpoint, int i, int cp_count, GLfloat* result)
{    
    int j;

    if ((i == 0 || i >= (cp_count - 1))) {
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
    }

    else { 
	for (j = 0; j<3; j++) 
	    result[j] = 0.5f * (cpoint[(i + 1) * 3 + j] -
				cpoint[(i - 1) * 3 + j]);
    }
}

void cubic_hermite_spline (GLfloat* cpoint, int i, int cp_count, 
			   GLfloat t, GLfloat* result)
{
    float h0, h1, h2, h3, tang1[3], tang2[3];

    h0 = 2*t*t*t - 3*t*t + 1;
    h1 = t*t*t - 2*t*t + t;
    h2 = -2*t*t*t + 3*t*t;
    h3 = t*t*t - t*t;

    tangent (cpoint, i, cp_count, tang1);
    tangent (cpoint, i + 1, cp_count, tang2);

    result[0] = h0*cpoint[i*3]		+ h1*tang1[0] +
		h2*cpoint[(i+1)*3]	+ h3*tang2[0];
    result[1] = h0*cpoint[i*3 + 1]	+ h1*tang1[1] +
		h2*cpoint[(i+1)*3 + 1]	+ h3*tang2[1];
    result[2] = h0*cpoint[i*3 + 2]	+ h1*tang1[2] +
		h2*cpoint[(i+1)*3 + 2]	+ h3*tang2[2];
}

void cubic_hermite_spline_normal (GLfloat* cpoint, int i, int cp_count, 
				  GLfloat t, GLfloat* result)
{
    GLfloat result_1[3], result_2[3], magnitude, temp[3];
    int j;

    cubic_hermite_spline (cpoint, i, cp_count, t, result_1);
    cubic_hermite_spline (cpoint, i, cp_count, t + 1e-5f, result_2);
    
    for (j=0; j<3; j++)
	result[j] = result_2[j] - result_1[j];

    magnitude = sqrt (result[0]*result[0] + result[2]*result[2]);

    for (j =0; j<3; j++)
	temp[j] = result[j] / magnitude;
    result[0] = temp[0]*cos(Pi / 2.0f) - temp[2]*sin(Pi / 2.0f);
    result[2] = temp[0]*sin(Pi / 2.0f) + temp[2]*cos(Pi / 2.0f);
}

GLfloat length (GLfloat* a, GLfloat* b)
{
    return sqrt (  pow (b[0] - a[0], 2) 
		+  pow (b[1] - a[1], 2) 
		+  pow (b[2] - a[2], 2));
}



