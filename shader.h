/*
 * =====================================================================================
 *
 *       Filename:  shader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/12/2011 07:00:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _SHADER_H
#define _SHADER_H

char* shader2string (const char* filename);
GLuint sh_loader (char* shader_source, GLenum type);
GLuint link_prg(GLuint vsh, GLuint fsh);

#endif

