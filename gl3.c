/*
 * =====================================================================================
 *
 *       Filename:  gl3.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/19/2011 06:40:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "matrix.h"
#include "shader.h"

GLuint* mk_vbo (mesh* obj)
{
    GLuint* buffer = malloc(2*sizeof(GLuint));
    glGenBuffers (2, buffer);
    glBindBuffer (GL_ARRAY_BUFFER, buffer[0]);
    glBufferData (GL_ARRAY_BUFFER, obj->vrtx_size, obj->vrtx, GL_STATIC_DRAW);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, obj->id_size, obj->id, GL_STATIC_DRAW);

    return buffer;
}

glres* mk_glres_from_obj (mesh* obj, char* vshader, char* fshader, 
			  char* attrib, char* unifrm_name, glarray uni_array,
			  GLfloat* model, GLfloat* view, GLfloat* projection,
			  GLenum mode)
{
    GLuint vbo, ibo;
    int i;

    glres* resource = malloc(sizeof(glres));
    resource->vbo   = malloc(sizeof(vertex_buffer)); 
    resource->vbo->attrb      = attrib;
    resource->vbo->type       = GL_ARRAY_BUFFER;
    resource->vbo->components = 3;

    glGenBuffers (1, &resource->vbo->id);
    glBindBuffer (resource->vbo->type, resource->vbo->id);
    glBufferData (resource->vbo->type, obj->vrtx_size, obj->vrtx, GL_STATIC_DRAW);
    
    if (obj->id != NULL) {
	glGenBuffers (1, &resource->ebo.id);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, resource->ebo.id);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, obj->id_size, obj->id, GL_STATIC_DRAW);
        resource->ebo.count       = (GLuint) obj->id_size/sizeof(GLuint);
    }
    else resource->ebo.count  = 0;

    resource->vbo->count      = (GLuint) obj->vrtx_size/sizeof(vertex);
    resource->vertex_shader   = sh_loader (vshader, GL_VERTEX_SHADER);
    resource->fragment_shader = sh_loader (fshader, GL_FRAGMENT_SHADER);
    resource->program         = link_prg (resource->vertex_shader, 
					  resource->fragment_shader);
    
    resource->vbo->attrb_loc = glGetAttribLocation (resource->program, 
						   resource->vbo->attrb);
    if (resource->vbo->attrb_loc == -1)
	printf ("\n \033[0;40;31m %s Attribute not found in shader program\n", 
							 resource->vbo->attrb);
    if (uni_array.data != NULL) {
	resource->uniform.name = unifrm_name;
	resource->uniform.array = uni_array;
	resource->uniform.loc = glGetUniformLocation (resource->program,
							    unifrm_name);
    }
    else {
	resource->uniform.name = NULL;
	resource->uniform.loc = -1; 
    }

    resource->vbo_count	      = 1;

    resource->model_matrix    = model;
    resource->view_matrix     = view;
    resource->prj_matrix      = projection;

    resource->mode = mode;

    return resource;
}

mesh* mkMesh_empty ()
{
    mesh* obj = malloc (sizeof(mesh));

    obj->vrtx = NULL;
    obj->id   = NULL;
    obj->clr  = NULL;
    
    obj->vrtx_size =0;
    obj->id_size   =0;
    obj->clr_size  =0;

    return obj;
}

glres* mk_empty_glres ()
{
    glres *resource = malloc (sizeof(glres));
    
    resource->vbo       = NULL;
    resource->vbo_count = 0;
    resource->ebo.id	= 0;
    resource->ebo.count = 0;
    
    resource->vertex_shader   = 0;
    resource->fragment_shader = 0;
    resource->program	      = 0;

    resource->uniform.name = NULL;
    resource->uniform.loc  = 0;
    resource->uniform.array.data = NULL;
    resource->uniform.array.count = 0;
    resource->model_matrix = NULL;
    resource->view_matrix  = NULL;
    resource->prj_matrix   = NULL;

    return resource;
}

glres* clone_of (glres* res)
{
    glres *resource = malloc (sizeof(glres));

    resource->vbo       = res->vbo;
    resource->vbo_count = res->vbo_count;
    resource->ebo	= res->ebo;

    resource->vertex_shader   = res->vertex_shader;
    resource->fragment_shader = res->fragment_shader;
    resource->program	      = res->program;

    resource->uniform	   = res->uniform;
    resource->model_matrix = res->model_matrix;
    resource->view_matrix  = res->view_matrix;
    resource->prj_matrix   = res->prj_matrix;

    return resource;
}


GLfloat* copy_matrix (GLfloat* matrix)
{
    int i;
    GLfloat* out_matrix = malloc (16*sizeof(GLfloat));

    for (i=0; i<16; i++) 
	out_matrix[i] = matrix[i];

    return out_matrix;
}

robj* mk_robj (glres* res, rnd_func rnd)
{
    robj* render_obj = malloc(sizeof(robj));
    render_obj->resource = res;
    render_obj->rendering = rnd;
    return render_obj;
}
    
void matrix_upload (GLuint id, GLfloat* projection, GLfloat* view, GLfloat* model)
{
    GLuint prj_location   = glGetUniformLocation (id, "projectionMatrix");
    GLuint view_location  = glGetUniformLocation (id, "viewMatrix");
    GLuint model_location = glGetUniformLocation (id, "modelMatrix");
    
    glUniformMatrix4fv (prj_location, 1, GL_FALSE, projection);
    glUniformMatrix4fv (view_location, 1, GL_FALSE, view);
    glUniformMatrix4fv (model_location, 1, GL_FALSE, model);
}

int render (glres* resource)
{
    glViewport (0, 0, 800, 600);
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0.2f, 0.0f, 0.2f, 0.0f);

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
    return 0;
}


