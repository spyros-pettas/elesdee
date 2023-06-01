/*
 * =====================================================================================
 *
 *       Filename:  background.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2011 07:34:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "structures.h"
#include "gl3.h"
#include "gradient.h"
#include "shader.h"
#include <stdlib.h>

glres* background_gradient ()
{
   mesh* back = malloc (sizeof(mesh));
   back->vrtx_size = 4*sizeof(vertex);
   back->vrtx = malloc (back->vrtx_size);

   back->vrtx[0].x = -1.0f; back->vrtx[0].y = -1.0f; back->vrtx[0].z = 0.0f;
   back->vrtx[1].x = 1.0f; back->vrtx[1].y = -1.0f; back->vrtx[1].z = 0.0f;
   back->vrtx[2].x = -1.0f; back->vrtx[2].y = 1.0f; back->vrtx[2].z = 0.0f;
   back->vrtx[3].x = 1.0f; back->vrtx[3].y = 1.0f; back->vrtx[3].z = 0.0f;

   back->id_size = 4*sizeof(GLuint);
   back->id = malloc (back->id_size);

   back->id[0] = 0;
   back->id[1] = 1;
   back->id[2] = 2;
   back->id[3] = 3;

   glres* back_res = malloc (sizeof(glres));
   back_res->vbo = malloc(sizeof(vertex_buffer));

   glGenBuffers (1, &back_res->vbo->id);
   glBindBuffer (GL_ARRAY_BUFFER, back_res->vbo->id);
   glBufferData (GL_ARRAY_BUFFER, back->vrtx_size, back->vrtx, GL_STATIC_DRAW);

   glGenBuffers (1, &back_res->ebo.id);
   glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, back_res->ebo.id);
   glBufferData (GL_ELEMENT_ARRAY_BUFFER, back->id_size, back->id, GL_STATIC_DRAW);

   back_res->ebo.count  = (GLuint) back->id_size/sizeof(GLuint);
   back_res->vbo->count = (GLuint) back->vrtx_size/sizeof(vertex);

   back_res->vertex_shader = sh_loader ("shaders/background.vert", GL_VERTEX_SHADER);
   back_res->fragment_shader = sh_loader ("shaders/background.frag", GL_FRAGMENT_SHADER);
   back_res->program = link_prg (back_res->vertex_shader, back_res->fragment_shader);
 

   back_res->vbo->attrb = "position";
   back_res->vbo->attrb_loc = glGetAttribLocation (back_res->program, back_res->vbo->attrb);

   back_res->model_matrix = NULL;
   back_res->view_matrix  = NULL;
   back_res->prj_matrix   = NULL;

   return back_res;
}

void background_render(glres* back_res)
{
    glViewport (0, 0, 800,600);
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_VERTEX_ARRAY);

    glClearColor (0.2f, 0.0f, 0.2f, 0.0f);


    glUseProgram (back_res->program);

    glBindBuffer (GL_ARRAY_BUFFER, back_res->vbo->id);
    glEnableVertexAttribArray (back_res->vbo->attrb_loc);
    glVertexAttribPointer (back_res->vbo->attrb_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, back_res->ebo.id);

    glDrawElements (GL_TRIANGLE_STRIP, back_res->ebo.count, GL_UNSIGNED_INT, (void*) 0);
    glDisableVertexAttribArray (back_res->vbo->attrb_loc);
    glUseProgram(0);
}



