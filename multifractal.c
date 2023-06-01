/*
 * =====================================================================================
 *
 *       Filename:  multifractal.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/2011 06:38:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "structures.h"
#include "perlin.h"
#include <GL/glfw.h>
#include <math.h>
#include <stdlib.h>

GLfloat Noise3 (GLfloat point[3])
{
    return (noise3(point));
}

GLfloat HybridMultiFractal ( GLfloat point[3], GLfloat H, 
			     GLfloat lacunarity,
			     GLfloat octaves, GLfloat offset )
{
    GLfloat frequency, result, signal, weight, remainder,p;
    int i;
    static int first = 1;
    static GLfloat* exponent_array;

    if (first) {
	exponent_array = malloc ( (octaves +1) *sizeof(GLfloat) );
	frequency = 1.0f;
	for (i=0; i<=octaves; i++) {
	    exponent_array[i] = pow (frequency, -H);
	    /*printf ("%f\n", exponent_array[i]);*/
	    frequency *= lacunarity;
	}
	first = 0;
    }

    result = (Noise3 (point) + offset) * exponent_array[0];
    weight = result;

    point[0] *= lacunarity;
    point[1] *= lacunarity;
    point[2] *= lacunarity;

    for (i=1; i<octaves; i++) {
	if (weight > 1.0f)
	    weight = 1.0f;
	
	signal  = (Noise3 (point) + offset)* exponent_array[i];
	result += weight*signal;
	weight *= signal;

	point[0] *= lacunarity;
	point[1] *= lacunarity;
	point[2] *= lacunarity;
    }

    remainder = octaves - (int) octaves;

    if (remainder)
	result += remainder*Noise3(point) * exponent_array[i];


    return result;
}

