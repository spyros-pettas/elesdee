/*
 * =====================================================================================
 *
 *       Filename:  collision.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/2011 07:52:22 PM
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
} 2d_vector;

GLfloat 2d_vector_length (2d_vector v)
{
    return v.x*v.x + v.z*v.z;
}

GLfloat 2d_dot( 2d_vector v, 2d vector n)
{
    return v.x*n.x + v.z*n.z;
}

GLuint check_collision_with_road (GLfloat x, GLfloat z, mesh* road)
{
    GLuint vrtx_count = road->vrtx_size/sizeof(GLuint);
    
    GLuint adaptive_flag = 1;
    GLuint index;
    
    while (adaptive_flag) {
	index = (GLuint) id_count/2;
	up_bound = vrtx_count;
	down_bound = 0;
    while (up_bound - down_bound > 2) {
	if (z<road->vrtx[index].z) {
	    up_bound = index;
	    down_bound = down_bound; 
	    index = (GLuint) (up_bound - down_bound)/2;
	}
	if (z > road->vrtx[index].z) {
	    up_bound = up_bound;
	    down_bound = index;
	    index = (GLuint) (up_bound - down_bound)/2;
	}
	
    }


