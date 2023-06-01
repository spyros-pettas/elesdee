/*
 * =====================================================================================
 *
 *       Filename:  fbo.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2011 08:21:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "structures.h"
#include "stdlib.h"
#include "fbo.h"

fbo* mk_fbo (GLenum target)
{

    fbo* fbuffer = malloc (sizeof(fbo));
    fbuffer->target = target;
    fbuffer->pixels = malloc (32*32*4*sizeof(GLubyte));

    glGenTextures (1, &fbuffer->tex);
    glBindTexture (GL_TEXTURE_2D, fbuffer->tex);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, 
		 GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap (GL_TEXTURE_2D);
    
    glGenFramebuffers (1, &fbuffer->id);
    glBindFramebuffer (fbuffer->target, fbuffer->id);
    glFramebufferTexture2D (fbuffer->target, GL_COLOR_ATTACHMENT0, 
			    GL_TEXTURE_2D, fbuffer->tex, 0);

    return fbuffer;
}

void render_to_fbo (glres* resource, fbo* fbuffer)
{
    glBindFramebuffer (GL_FRAMEBUFFER, fbuffer->id);
    glViewport (0, 0, 32, 32);
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_VERTEX_ARRAY);
    glEnable (GL_CULL_FACE);
    //glEnable (GL_BLEND);
    //glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);

    glUseProgram (resource->program);
    matrix_upload (resource->program, resource->prj_matrix, 
		   resource->view_matrix, resource->model_matrix);
    
    int i=0;
    for (i=0; i<resource->vbo_count; i++) {
	glBindBuffer (GL_ARRAY_BUFFER, resource->vbo[i].id);
	glEnableVertexAttribArray (resource->vbo[i].attrb_loc);
	glVertexAttribPointer (resource->vbo[i].attrb_loc, 
			       resource->vbo[i].components, 
			        GL_FLOAT, GL_FALSE, 0, 0);

    }
    
    if (resource->ebo.count != 0) {
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, resource->ebo.id);
	glDrawElements (resource->mode, resource->ebo.count, GL_UNSIGNED_INT, (void*) 0);
    }
    else glDrawArrays (GL_TRIANGLE_STRIP, 0, resource->vbo->count);

    if (resource->uniform.name != NULL) 
	glUniform4fv (resource->uniform.loc, resource->uniform.array.count, resource->uniform.array.data);

    for (i=0;i<resource->vbo_count;i++)
	glDisableVertexAttribArray (resource->vbo[i].attrb_loc);
    glUseProgram(0);

    glReadPixels (0, 0, 32, 32, GL_BGRA, GL_UNSIGNED_BYTE, fbuffer->pixels);

    glBindFramebuffer (GL_FRAMEBUFFER_EXT, 0);
}

glres* mk_fbo_res ()
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

   tex_coord* coord = malloc(4*sizeof(tex_coord));

   coord[0].u = 0.0f; coord[0].v = 0.0f;
   coord[1].u = 1.0f; coord[0].v = 0.0f;
   coord[2].u = 0.0f; coord[2].v = 1.0f;
   coord[3].u = 1.0f; coord[3].v = 1.0f;

   glres* back_res = malloc (sizeof(glres));
   back_res->vbo = malloc(2*sizeof(vertex_buffer));

   glGenBuffers (1, &back_res->vbo[0].id);
   glBindBuffer (GL_ARRAY_BUFFER, back_res->vbo[0].id);
   glBufferData (GL_ARRAY_BUFFER, back->vrtx_size, back->vrtx, GL_STATIC_DRAW);

   glGenBuffers (1, &back_res->vbo[1].id);
   glBindBuffer (GL_ARRAY_BUFFER, back_res->vbo[1].id);
   glBufferData (GL_ARRAY_BUFFER, 4*sizeof(tex_coord), coord, GL_STATIC_DRAW);

   glGenBuffers (1, &back_res->ebo.id);
   glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, back_res->ebo.id);
   glBufferData (GL_ELEMENT_ARRAY_BUFFER, back->id_size, back->id, GL_STATIC_DRAW);

   back_res->ebo.count  = (GLuint) back->id_size/sizeof(GLuint);
   back_res->vbo[0].count = (GLuint) back->vrtx_size/sizeof(vertex);
   back_res->vbo[1].count = 4;

   back_res->vertex_shader = sh_loader ("shaders/fbo.vert", GL_VERTEX_SHADER);
   back_res->fragment_shader = sh_loader ("shaders/fbo.frag", GL_FRAGMENT_SHADER);
   back_res->program = link_prg (back_res->vertex_shader, back_res->fragment_shader);
 

   back_res->vbo[0].attrb = "position";
   back_res->vbo[0].attrb_loc = glGetAttribLocation (back_res->program, back_res->vbo[0].attrb);

   back_res->vbo[1].attrb = "texcoord";
   back_res->vbo[1].attrb_loc = glGetAttribLocation (back_res->program, back_res->vbo[1].attrb);

   back_res->model_matrix = NULL;
   back_res->view_matrix  = NULL;
   back_res->prj_matrix   = NULL;

   return back_res;
}

void fbo_render(glres* fbo_res, fbo* fbuffer)
{
    glViewport (0, 0, 800,600);
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_VERTEX_ARRAY);

    glClearColor (0.2f, 0.0f, 0.2f, 1.0f);
    
    glEnable (GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glUseProgram (fbo_res->program);

    GLint loc = glGetUniformLocation (fbo_res->program, "texture");
    glUniform1ui (loc, 32*32*4, fbuffer->pixels);

    glBindBuffer (GL_ARRAY_BUFFER, fbo_res->vbo[0].id);
    glEnableVertexAttribArray (fbo_res->vbo[0].attrb_loc);
    glVertexAttribPointer (fbo_res->vbo[0].attrb_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer (GL_ARRAY_BUFFER, fbo_res->vbo[1].id);
    glEnableVertexAttribArray (fbo_res->vbo[1].attrb_loc);
    glVertexAttribPointer (fbo_res->vbo[1].attrb_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, fbo_res->ebo.id);

    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, fbuffer->tex);

    glDrawElements (GL_TRIANGLE_STRIP, fbo_res->ebo.count, GL_UNSIGNED_INT, (void*) 0);
    glDisableVertexAttribArray (fbo_res->vbo->attrb_loc);
    glUseProgram(0);
    glDisable (GL_BLEND);
}


