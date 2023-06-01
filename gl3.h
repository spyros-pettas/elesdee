/*
 * =====================================================================================
 *
 *       Filename:  gl3.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/14/2011 06:37:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _GL3_H
#define _GL3_H

GLuint* mk_vbo (mesh* obj);
mesh* mkMesh_empty ();
glres* mk_glres_from_obj (mesh* obj, char* vshader, char* fshader, char* attrib,
			  char* unifrm_name, glarray uni_array,
			  GLfloat* model, GLfloat* view, GLfloat* projection,
			  GLenum mode);
void matrix_upload (GLuint id, GLfloat* projection, GLfloat* view, GLfloat* model);
GLuint attribute_upload (glres resource, GLuint id);
robj* mk_robj (glres* res, rnd_func rnd);
int  render (glres* resource);

#endif




