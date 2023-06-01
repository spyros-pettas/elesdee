/*
 * =====================================================================================
 *
 *       Filename:  ply_loader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2011 08:27:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _PLY_LOADER_H
#define _PLY_LOADER_H

mesh* ply_loader(char* filename);
glres* mk_glres_from_ply (mesh* iobj, char* vshader, char* fshader, 
			  char* vrtx_attrb, char* clr_attrb,
			  GLfloat* model, GLfloat* view, GLfloat* projection,
			  GLenum mode);


#endif
