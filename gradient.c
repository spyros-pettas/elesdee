/*
 * =====================================================================================
 *
 *       Filename:  gradient.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/11/2011 01:18:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "structures.h"
#include <stdlib.h>

glarray mk_gradient(gradient_cp* cp, GLuint resolution, GLuint Count)
{
    int i, j, k, slope;
    GLuint grad_subset[Count];

    glarray gradient;
    gradient.data  = malloc(resolution*(sizeof(color)));

    for (i=0; i<Count; i++) {
	grad_subset[i] = cp->pos[i]*resolution;
    }

    for (i = 0; i<(Count-1); i++) {
	
	j = grad_subset[i];
	gradient.data[j]   = cp->clr[i].r;
	gradient.data[j+1] = cp->clr[i].g;
	gradient.data[j+2] = cp->clr[i].b;
	gradient.data[j+3] = cp->clr[i].a;
	slope = grad_subset[i+1] - grad_subset[i];

	k = grad_subset[i]; 
	while (k < grad_subset[i+1]) {
	   	    
	    gradient.data[k] = gradient.data[j] + 
			  ((cp->clr[i+1].r - cp->clr[i].r)/slope)*(k - j);

	    gradient.data[k+1] = gradient.data[j+1] + 
			  ((cp->clr[i+1].g - cp->clr[i].g)/slope)*(k - j);

	    gradient.data[k+2] = gradient.data[j+2] + 
			  ((cp->clr[i+1].b - cp->clr[i].b)/slope)*(k - j);

	    gradient.data[k+3] = gradient.data[j+3] + 
			  ((cp->clr[i+1].a - cp->clr[i].a)/slope)*(k - j);

	    k=k+4;
	}

    
    }
    gradient.count = resolution/4;
    return gradient;
}

