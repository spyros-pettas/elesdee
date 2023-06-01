/*
 * =====================================================================================
 *
 *       Filename:  obj_loader.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/06/2011 11:40:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "structures.h"
#include <GL/glfw.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void obj_loader (mesh* obj, const char* filename)
{
    unsigned long vi, vni, vfi, vertex_lines = 0, vnormal_lines = 0, faces = 0;
    char line_id[3], buffer[256];
    GLfloat x = 0, y = 0, z =0;
    GLuint f1 = 0, f2 = 0, f3 = 0, fnull;
    FILE* pfile = NULL;

    pfile = fopen (filename, "r");
    if (pfile == NULL) printf ("no file found\n");

    while (!feof(pfile)) {
	memset (buffer ,0 ,256);
	memset (line_id, 0, 3);
	fgets (buffer, 256, pfile);
	memcpy (line_id, buffer, 3);
	if (strncmp (line_id, "v ", 2) == 0) vertex_lines++;
	if (strncmp (line_id, "vn ", 3) == 0) vnormal_lines++;
	if (strncmp (line_id, "f ", 2) == 0) faces++;
    }

    obj->vrtx_size = vertex_lines*sizeof(vertex);
    obj->id_size = 3*faces*sizeof(GLuint);
    obj->vrtx = malloc (obj->vrtx_size);
    obj->id = malloc (obj->id_size);
    printf("\n%u\n", ( (GLuint) (obj->id_size/(sizeof(GLuint)))));
    rewind (pfile);
    vni = 0;
    vfi = 0;
    vi = 0;

    while (!feof (pfile)) {
	memset (buffer, 0, 256);
	memset (line_id, 0, 3);
	fgets (buffer, 256, pfile);
	memcpy (line_id, buffer, 3);
	if (strncmp (line_id, "v ", 2) == 0) {
	    sscanf (buffer, "%s%f%f%f", line_id, &x, &y, &z);
	    obj->vrtx[vi].x = x;
	    obj->vrtx[vi].y = y;
	    obj->vrtx[vi].z = z;
	    vi++;
	}
    
	if (strncmp (line_id, "f ", 2) == 0) {
	    sscanf( buffer, "%s %u//%u %u//%u %u//%u", line_id, 
			&f1, &fnull, &f2, &fnull, &f3, &fnull);
	    
	    obj->id[3*vfi] = f1 - 1;
	    obj->id[3*vfi + 1] = f2 - 1;
	    obj->id[3*vfi + 2] = f3 - 1;
	    vfi = vfi++;
	}
    }    
}
