/*
 * =====================================================================================
 *
 *       Filename:  space_partition.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/2011 06:12:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "structures.h"

typedef struct {
    GLfloat x;
    GLfloat z;
} partition;

partition* partition (spline* road, GLuint subdiv)
{
    GLuint parts = (GLuint) road->count / subdiv;

    GLuint part[];
    GLuint i = 0;

    for (i=0;i<parts;i++)
	part[i] = i;


