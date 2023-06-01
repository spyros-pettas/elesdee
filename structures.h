/*
 * =====================================================================================
 *
 *       Filename:  structures.h
 *
 *    Description:  Structures header file
 *
 *        Version:  0.06
 *        Created:  07/19/2011 06:34:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Spyros Pettas
 *        Company:  Gray Alien Resistance
 *
 * =====================================================================================
 */

#ifndef _STRUCTURES_H
#define _STRUCTURES_H
#include <GL/glfw.h>

typedef struct
{		
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vertex;

typedef struct
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
} color;

typedef struct
{
    color* clr;
    GLfloat* pos;
} gradient_cp;

typedef struct
{
    vertex* vrtx;
    size_t vrtx_size;
} norm;

typedef struct
{
    GLfloat u;
    GLfloat v;
} tex_coord;

typedef struct
{
    GLuint *id;
    vertex *vrtx;
    color* clr;
    tex_coord* tex;
    size_t id_size;
    size_t vrtx_size;
    size_t clr_size;
    size_t tex_size;

} mesh;

typedef struct 
{
    vertex* v;
    vertex* n;
    GLuint count;
} spline;

typedef struct
{
    vertex img;
    GLfloat angle;
} quatrn;

typedef struct
{
    GLuint id;
    GLuint count;
} element_buffer;

typedef struct
{
    GLuint id;
    char* attrb;
    GLint attrb_loc;
    GLenum type;
    GLint components;
    GLsizei stride;
    GLuint count;
} vertex_buffer;

typedef struct
{
    GLfloat* data;
    GLuint count;
} glarray;

typedef struct
{
    char* name;
    GLint loc;
    glarray array;
} uniform4fv;

typedef struct
{
    vertex_buffer* vbo;
    GLuint vbo_count;
    element_buffer ebo;
    
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
    
    uniform4fv uniform;

    GLfloat* model_matrix;
    GLfloat* view_matrix;
    GLfloat* prj_matrix;
    GLenum mode;
} glres;

typedef struct
{
    glres** resource;
    GLuint count;
} render_list;

typedef int (*rnd_func) (glres*);

typedef struct
{
    glres* resource;
    rnd_func rendering;
} robj;

typedef struct
{
    int count;
    glres* res;
} display_list;

#endif
