/*
 * =====================================================================================
 *
 *       Filename:  procdl.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2011 07:43:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _PROCDL_H
#define _PROCDL_H

GLfloat smooth_step (float a, float b, float x);
void tangent (GLfloat* cpoint, int i, int cp_count, GLfloat* result);
void cubic_hermite_spline (GLfloat* cpoint, int i, int  cp_count,
			   GLfloat t, GLfloat* result);

void cubic_hermite_spline_normal (GLfloat* cpoint, int i, int cp_count, 
				  GLfloat t, GLfloat* result);
GLfloat length (GLfloat* a, GLfloat* b);



#endif
