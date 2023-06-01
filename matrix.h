/*
 * =====================================================================================
 *
 *       Filename:  matrix.h
 *
 *    Description:  Transform/View/Projection Matrix Definition
 *
 *        Version:  1.0
 *        Created:  06/30/2011 05:57:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Spyros Pettas
 *        Company:  Gray Alien Resistance
 *
 * =====================================================================================
 */
#ifndef _MATRIX_H
#define _MATRIX_H

GLfloat* mk_prjmatrix (GLfloat fovy, GLfloat aspect, GLfloat znear, GLfloat zfar);

GLfloat* mk_mdlmatrix (GLfloat x, GLfloat y, GLfloat z,
				    quatrn* quaternion);

void translate (glres* resource, GLfloat dx, GLfloat dy, GLfloat dz);

quatrn* mk_quatrn (GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void update_quatrn (quatrn* quaternion, GLfloat x, GLfloat y, 
					GLfloat z, GLfloat w);

void quatrn_normalize (quatrn* quaternion);

void localize_quatrn(quatrn* quaternion);

void rotate (glres* resource, quatrn* quaternion);

GLfloat* mk_inverse_matrix (GLfloat* mat);

GLfloat* mk_camera (GLfloat xi, GLfloat yi, GLfloat zi,
				    quatrn* quaternion);

void cam_transform (GLfloat* camera, GLfloat xi, GLfloat yi, 
		    GLfloat zi, quatrn* quaternion);

void cam_position  (GLfloat* camera, GLfloat xi, GLfloat yi, 
		    GLfloat zi, quatrn* quaternion);

vertex cross_product (vertex b, vertex c);

void normalize_vector (vertex *a);

GLuint compare_vertices (vertex* a, vertex* b);

void assign_vertices (vertex* a, vertex* b);



#endif
