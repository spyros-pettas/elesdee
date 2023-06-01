/*
 * =====================================================================================
 *
 *       Filename:  fbo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2011 06:12:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _FBO_H
#define _FBO_H

typedef struct 
{
    GLenum target;
    GLuint id;
    GLuint tex;
    GLubyte* pixels;
} fbo;

fbo* mk_fbo (GLenum target);
void render_to_fbo (glres* resource, fbo* fbuffer);
glres* mk_fbo_res ();
void fbo_render(glres* fbo_res, fbo* fbuffer);

#endif
